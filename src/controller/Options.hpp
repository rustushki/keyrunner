#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "../model/State.hpp"

class Options {
public:
    // Parsers
    static void parse(int argc, char** argv);

    // Output functions
    static void showHelp();
    static void showVersion();

    // Accessors
    static uint16_t getStartingLevel();
    static State getInitialState();
    static bool getCreateNewLevel();

private:

    static uint16_t startingLevelNum;
    static State initialState;
    static bool createNewLevel;
    static bool isPositiveInt(char* str);
    static void die(std::string msg);

};

#endif
