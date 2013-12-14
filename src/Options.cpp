#include <iostream>
#include <cstring>
#include <cstdlib>

#include "KeyRunner.hpp"
#include "Options.hpp"
#include "State.hpp"

uint16_t Options::startingLevelNum = 1;
State Options::initialState = PLAY;

void Options::parse(int argc, char** argv) {
    if (argc > 1) {
        // Handle Starting Level Option
        if        (    strcmp(argv[1], "--level") == 0
                    || strcmp(argv[1], "-l") == 0) {
            if (argc < 3) {
                std::cout << "You must provide a level number." << std::endl;
                exit(0);
            }
            startingLevelNum = atoi(argv[2]);

        // Handle Help Flags.
        } else if (    strcmp(argv[1], "--help") == 0
                    || strcmp(argv[1], "-h") == 0) {
            showHelp();

        // Handle Version Flags.
        } else if (    strcmp(argv[1], "--version") == 0
                    || strcmp(argv[1], "-v") == 0 ) {
            showVersion();
        }
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
    std::cout << "Start in Editor Mode:" << std::endl;
    std::cout << "   keyrunner --editor" << std::endl;
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
