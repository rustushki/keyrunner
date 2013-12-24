#ifndef OPTIONS_HPP
#define OPTIONS_HPP

class Options {
public:
    // Parsers
    static void parse(int argc, char** argv);

    // Outputters
    static void showHelp();
    static void showVersion();

    // Accessors
    static uint16_t getStartingLevel();
    static State getInitialState();

private:

    static uint16_t startingLevelNum;
    static State initialState;
    static bool isPositiveInt(char* str);
    static void die(std::string msg);

};

#endif//OPTIONS_HPP
