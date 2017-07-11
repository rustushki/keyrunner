#include <iostream>
#include <cstring>
#include <sstream>

#include "../controller/KeyRunner.hpp"
#include "../controller/Options.hpp"
#include "../model/LevelManager.hpp"

/**
 * Constructor.
 */
Options::Options() {
    startingLevelNum = 1;
    initialState = TITLE;
    createNewLevel = false;
}

/**
 * Parse the command line arguments provided.
 * <p>
 * Populates local static variables for this class scope with input obtained from the commandline.  This information is
 * obtainable from accessors.
 *
 * @param argc count of arguments
 * @param argv array of string arguments
 */
void Options::parse(int argc, char** argv) {
    bool isLevelNotSet = true;

    for (int argumentX = 1; argumentX < argc; argumentX++) {
        // Handle Starting Level Option
        if (strcmp(argv[argumentX], "--level") == 0 || strcmp(argv[argumentX], "-l") == 0) {
            // Confirm that a level number has been provided.
            if (argumentX + 1 >= argc || !isPositiveInt(argv[argumentX + 1])) {
                die("You must provide a level number.");
            }

            // Confirm that the provided level is within range and is thus valid.
            startingLevelNum = (uint8_t) atoi(argv[++argumentX]);
            if (startingLevelNum < 1 || startingLevelNum > 0xFF) {
                die("Level out of range.");
            }

            isLevelNotSet = false;

        // Handle Help Flags.
        } else if (strcmp(argv[argumentX], "--help") == 0 || strcmp(argv[argumentX], "-h") == 0) {
            showHelp();

        // Handle Version Flags.
        } else if (strcmp(argv[argumentX], "--version") == 0 || strcmp(argv[argumentX], "-v") == 0 ) {
            showVersion();

        // Initial Mode will be the Editor.
        } else if (strcmp(argv[argumentX], "--editor") == 0 || strcmp(argv[argumentX], "-e") == 0 ) {
            initialState = EDIT;

        // Initial Mode will be the Editor.
        } else if (strcmp(argv[argumentX], "--new") == 0 || strcmp(argv[argumentX], "-n") == 0 ) {
            createNewLevel = true;

        } else {
            die("Unrecognized option: " + std::string(argv[argumentX]));
        }
    }

    if (initialState == EDIT && isLevelNotSet && !createNewLevel) {
        die("You must provide a level number to edit or specify -n.");
    }

	// If user sets a level, and the mode isn't EDIT, assume they want to play immediately
    if (initialState != EDIT && !isLevelNotSet) {
        initialState = PLAY;
    }
}

/**
 * Present the help text to the console.
 */
void Options::showHelp() {
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
void Options::showVersion() {
    std::stringstream ss;
    ss << "KeyRunner version " << VERSION;
    die(ss.str());
}

/**
 * Fetch the starting level provided from the command line (default = 1).
 * @return starting level
 */
uint8_t Options::getStartingLevel() {
    return startingLevelNum;
}

/**
 * Fetch the state the game should start in (default = PLAY).
 * @return PLAY or EDIT
 */
State Options::getInitialState() {
    return initialState;
}

/**
 * Did the user indicate that they want to create a new level?
 * @return did they?
 */
bool Options::getCreateNewLevel() {
    return createNewLevel;
}

/**
 * Return boolean indicating whether a string is a positive integer or not.
 * @param str the string
 * @return was this string a positive integer?
 */
bool Options::isPositiveInt(char* str) {
    bool passes = true;
    for (int x = 0; str[x] != 0; x++) {
        if (!std::isdigit(str[x])) {
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
void Options::die(std::string msg) {
    std::cout << msg << std::endl;
    exit(0);
}
