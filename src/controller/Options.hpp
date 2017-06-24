#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "../model/State.hpp"

class Options {
public:
    Options();

    // Parsers
    void parse(int argc, char** argv);

    // Output functions
    void showHelp();
    void showVersion();

    // Accessors
    uint8_t getStartingLevel();
    State getInitialState();
    bool getCreateNewLevel();

private:

    uint8_t startingLevelNum;
    State initialState;
    bool createNewLevel;
    bool isPositiveInt(char* str);
    void die(std::string msg);
};

#endif
