#include <iostream>
#include <cstring>
#include <sstream>

#include "../controller/KeyRunner.hpp"
#include "../controller/OptionController.hpp"

/**
 * Constructor.
 */
OptionController::OptionController(OptionModel* model, int argc, char** argv) {
    this->optionModel = model;
    this->argc = argc;
    this->argv = argv;
}


/**
 * Present the help text to the console.
 */
void OptionController::showHelp() {
    std::stringstream ss;
    ss << "usage: KeyRunner [-v|--version] [-h|--help]" << std::endl;
    ss << "                 [-e|--editor] [-n|--new]" << std::endl;
    ss << "                 [-l|--level <args>]" << std::endl;
    ss << "" << std::endl;
    ss << "Hints:" << std::endl;
    ss << "" << std::endl;
    ss << "Start Game Normally:" << std::endl;
    ss << "   KeyRunner" << std::endl;
    ss << "" << std::endl;
    ss << "Start in Editor Mode (editing a brand new level):" << std::endl;
    ss << "   KeyRunner --editor --new" << std::endl;
    ss << "" << std::endl;
    ss << "Start in Editor Mode (editing level 27):" << std::endl;
    ss << "   KeyRunner --editor --level 27" << std::endl;
    ss << "" << std::endl;
    ss << "Start at a Specific Level:" << std::endl;
    ss << "   KeyRunner --level 27" << std::endl;
    ss << "" << std::endl;
    ss << "Enjoy!" << std::endl;
    die(ss.str());
    exit(0);
}

/**
 * Present the version number to the console.
 */
void OptionController::showVersion() {
    std::stringstream ss;
    ss << "KeyRunner version " << VERSION;
    die(ss.str());
}

/**
 * Return boolean indicating whether a string is a positive integer or not.
 * @param str the string
 * @return was this string a positive integer?
 */
bool OptionController::isPositiveInt(char* str) {
    bool passes = true;
    for (int x = 0; str[x] != 0; x++) {
        if (std::isdigit(str[x]) == 0) {
            passes = false;
            break;
        }
    }
    return passes;
}

/**
 * Exit the program with a message.
 * @param msg the message to die with
 */
void OptionController::die(std::string msg) {
    std::cout << msg << std::endl;
    exit(0);
}

/**
 * Fetch the OptionModel associated with this Controller
 * @return OptionModel*
 */
OptionModel* OptionController::getModel() const {
    return optionModel;
}

/**
 * Executes the OptionController.
 * <p>
 * This is not so much a loop. It simply updates the model once.
 */
void OptionController::gameLoop() {
    updateModel(0);
}

/**
 * Read each command line option and update the model.
 * <p>
 * If a command line option requires immediate action, take it.
 * @param frameDuration
 */
void OptionController::updateModel(long frameDuration) {
    bool isLevelNotSet = true;

    for (int argumentX = 1; argumentX < argc; argumentX++) {
        // Handle Starting Level Option
        if (strcmp(argv[argumentX], "--level") == 0 || strcmp(argv[argumentX], "-l") == 0) {
            // Confirm that a level number has been provided.
            if (argumentX + 1 >= argc || !isPositiveInt(argv[argumentX + 1])) {
                die("You must provide a level number.");
            }

            // Confirm that the provided level is within range and is thus valid.
            auto startingLevel = static_cast<uint8_t>(atoi(argv[++argumentX]));
            if (startingLevel < 1 || startingLevel > 0xFF) {
                die("Level out of range.");
            }

            getModel()->setStartingLevel(startingLevel);

            isLevelNotSet = false;

        // Handle Help Flags.
        } else if (strcmp(argv[argumentX], "--help") == 0 || strcmp(argv[argumentX], "-h") == 0) {
            showHelp();

        // Handle Version Flags.
        } else if (strcmp(argv[argumentX], "--version") == 0 || strcmp(argv[argumentX], "-v") == 0 ) {
            showVersion();

        // Initial Mode will be the Editor.
        } else if (strcmp(argv[argumentX], "--editor") == 0 || strcmp(argv[argumentX], "-e") == 0 ) {
            getModel()->setInitialState(EDIT);

        // Initial Mode will be the Editor.
        } else if (strcmp(argv[argumentX], "--new") == 0 || strcmp(argv[argumentX], "-n") == 0 ) {
            getModel()->setCreateNewLevel(true);

        } else {
            die("Unrecognized option: " + std::string(argv[argumentX]));
        }
    }

    if (getModel()->getInitialState() == EDIT && isLevelNotSet && !getModel()->getCreateNewLevel()) {
        die("You must provide a level number to edit or specify -n.");
    }

	// If user sets a level, and the mode isn't EDIT, assume they want to play immediately
    if (getModel()->getInitialState() != EDIT && !isLevelNotSet) {
        getModel()->setInitialState(PLAY);
    }
}

/**
 * This function isn't really used.
 * <p>
 * Since there is no loop, it may always return false.
 * @return boolean
 */
bool OptionController::isStillExecuting() const {
    return false;
}
