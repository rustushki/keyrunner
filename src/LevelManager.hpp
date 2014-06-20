#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include <vector>

#include "TileType.hpp"

class Level;
class TileLayer;

class LevelManager {
public:
    static uint16_t GetTotal();
    static bool Exists(uint8_t levelNum);
    static Level* Read(uint8_t levelNum);
    static bool Write(Level* level);
    static Level* New(uint8_t levelNum);

private:
    static std::string GetPath(uint8_t levelNum, bool inCwd);
    static void Reset();

    // Populate uses the data below to 'fill in' a Level object.
    static void Populate(Level* level);
    static uint16_t w;
    static uint16_t h;
    static TileType defTT;
    static std::vector<TileLayer*> deviations;
    static uint16_t px;
    static uint16_t py;
    static uint16_t kx;
    static uint16_t ky;
};

#endif//LEVELMANAGER_HPP
