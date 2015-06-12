#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sstream>

#include "KeyRunner.hpp"
#include "Options.hpp"
#include "../model/LevelManager.hpp"
#include "../model/State.hpp"

uint16_t Options::startingLevelNum = 1;
State Options::initialState = PLAY;
bool Options::createNewLevel = false;

void Options::parse(int argc, char** argv) {
    bool isLevelNotSet = true;

    for (int argx = 1; argx < argc; argx++) {
        // Handle Starting Level Option
        if        (    strcmp(argv[argx], "--level") == 0
                    || strcmp(argv[argx], "-l") == 0) {
            // Confirm that a level number has been provided.
            if (argx + 1 >= argc || !isPositiveInt(argv[argx + 1])) {
                die("You must provide a level number.");
            }

            // Confirm that the provided level is within range and is thus
            // valid.
            startingLevelNum = atoi(argv[++argx]);
            if (startingLevelNum < 1 || startingLevelNum > LevelManager::GetTotal()) {
                die("Invalid level.");
            }

            isLevelNotSet = false;

        // Handle Help Flags.
        } else if (    strcmp(argv[argx], "--help") == 0
                    || strcmp(argv[argx], "-h") == 0) {
            showHelp();

        // Handle Version Flags.
        } else if (    strcmp(argv[argx], "--version") == 0
                    || strcmp(argv[argx], "-v") == 0 ) {
            showVersion();

        // Initial Mode will be the Editor.
        } else if (    strcmp(argv[argx], "--editor") == 0
                    || strcmp(argv[argx], "-e") == 0 ) {
            initialState = EDIT;

        // Initial Mode will be the Editor.
        } else if (    strcmp(argv[argx], "--new") == 0
                    || strcmp(argv[argx], "-n") == 0 ) {
            createNewLevel = true;

        } else {
            die("Unrecognized option: " + std::string(argv[argx]));
        }
    }

    if (initialState == EDIT && isLevelNotSet && !createNewLevel) {
        die("You must provide a level number to edit or specify -n.");
    }

}

void Options::showHelp() {
    std::stringstream ss;
    ss << "usage: keyrunner [-v|--version] [-h|--help]" << std::endl;
    ss << "                 [-e|--editor] [-n|--new]" << std::endl;
    ss << "                 [-l|--level <args>]" << std::endl;
    ss << "" << std::endl;
    ss << "Hints:" << std::endl;
    ss << "" << std::endl;
    ss << "Start Game Normally:" << std::endl;
    ss << "   keyrunner" << std::endl;
    ss << "" << std::endl;
    ss << "Start in Editor Mode (editing a brand new level):" << std::endl;
    ss << "   keyrunner --editor --new" << std::endl;
    ss << "" << std::endl;
    ss << "Start in Editor Mode (editing level 27):" << std::endl;
    ss << "   keyrunner --editor --level 27" << std::endl;
    ss << "" << std::endl;
    ss << "Start at a Specific Level:" << std::endl;
    ss << "   keyrunner --level 27" << std::endl;
    ss << "" << std::endl;
    ss << "Enjoy!" << std::endl;
    die(ss.str());
    exit(0);
}

void Options::showVersion() {
    std::stringstream ss;
    ss << "keyrunner version " << VERSION;
    die(ss.str());
}

uint16_t Options::getStartingLevel() {
    return startingLevelNum;
}

State Options::getInitialState() {
    return initialState;
}

bool Options::getCreateNewLevel() {
    return createNewLevel;
}

/* ------------------------------------------------------------------------------
 * isPositiveInt - Return boolean indicating whether a string is a positive
 * integer or not.
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

/* ------------------------------------------------------------------------------
 * die - Exit the program with a message.
 */
void Options::die(std::string msg) {
    std::cout << msg << std::endl;
    exit(0);
}
