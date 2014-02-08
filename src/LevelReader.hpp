#ifndef LEVELREADER_HPP
#define LEVELREADER_HPP

#include "KeyRunner.hpp"

class Level;

class LevelReader {
public:
    static Level* Read(uint8_t levelNum);
};

#endif//LEVELREADER_HPP
