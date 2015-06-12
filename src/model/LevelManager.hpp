#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include <string>
#include <map>
#include <vector>

#include "PlayModel.hpp"
#include "../game/TileType.hpp"

class Level;

class LevelManager {
public:
    static uint16_t GetTotal();
    static bool Exists(uint8_t levelNum);
    static void Read(uint8_t levelNum);
    static bool Write();
    static void New(uint8_t levelNum);

private:
    static std::string GetPath(uint8_t levelNum, bool inCwd);
    static void Reset();

    // Populate uses the data below to 'fill in' a Level object.
    static void Populate(uint8_t levelNum);
    static uint16_t w;
    static uint16_t h;
    static TileType defTT;
    static std::map<TileCoord, TileType> deviations;
    static TileCoord playerCoord;
    static TileCoord keyCoord;
};

#endif//LEVELMANAGER_HPP
