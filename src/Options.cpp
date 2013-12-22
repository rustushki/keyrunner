#include <iostream>
#include <cstring>
#include <cstdlib>

#include "KeyRunner.hpp"
#include "Options.hpp"
#include "State.hpp"
#include "LevelLoader.hpp"

uint16_t Options::startingLevelNum = 1;
State Options::initialState = PLAY;

void Options::parse(int argc, char** argv) {
    bool isLevelNotSet = true;

    for (int argx = 1; argx < argc; argx++) {
        // Handle Starting Level Option
        if        (    strcmp(argv[argx], "--level") == 0
                    || strcmp(argv[argx], "-l") == 0) {
            // Confirm that a level number has been provided.
            if (argx + 1 >= argc || !isPositiveInt(argv[argx + 1])) {
                std::cout << "You must provide a level number." << std::endl;
                exit(0);
            }

            // Confirm that the provided level is within range and is thus
            // valid.
            startingLevelNum = atoi(argv[++argx]);
            if (startingLevelNum < 1 || startingLevelNum > LevelLoader::GetTotal()) {
                std::cout << "Invalid level." << std::endl;
                exit(0);
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
        }
    }

    if (initialState == EDIT && isLevelNotSet) {
        std::cout << "You must provide a level number to edit." << std::endl;
        exit(0);
    }

}

void Options::showHelp() {
    std::cout << "usage: keyrunner [-v|--version] [-h|--help]" << std::endl;
    std::cout << "                 [-l|--level] [-e|--editor]" << std::endl;
    std::cout << "                 [<args>]" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Hints:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Start Game Normally:" << std::endl;
    std::cout << "   keyrunner" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Start in Editor Mode (editing a brand new level):" << std::endl;
    std::cout << "   keyrunner --editor" << std::endl;
    std::cout << "Start in Editor Mode (editing level 27):" << std::endl;
    std::cout << "   keyrunner --editor --level 27" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Start at a Specific Level:" << std::endl;
    std::cout << "   keyrunner --level 27" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Enjoy!" << std::endl;
    exit(0);
}

void Options::showVersion() {
    std::cout << "keyrunner version " << VERSION << std::endl;
    exit(0);
}

uint16_t Options::getStartingLevel() {
    return startingLevelNum;
}

State Options::getInitialState() {
    return initialState;
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
