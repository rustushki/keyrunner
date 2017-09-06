#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include <string>
#include <map>
#include <vector>

#include "../model/BoardModel.hpp"
#include "../model/TileType.hpp"

class Level;

class LevelManager {
public:
    explicit LevelManager(BoardModel* board);
    uint8_t getLevelCount() const;
    bool levelExists(uint8_t levelNumber) const;
    void read();
    void write() const;
    void create();

private:
    std::string getPath(uint8_t levelNumber, bool inCurrentWorkingDirectory) const;
    void resetLevelManager();
    void populateBoard();

    void readSize(FILE *fp);
    void readDefaultTileType(FILE *fp);
    void readInitialPlayerCoordinate(FILE *fp);
    void readDeviations(FILE *fp);
    void readItems(FILE *fp);

    void writeSize(FILE* fp) const;
    void writeDefaultTileType(FILE* fp) const;
    void writeDeviations(FILE* fp) const;
    void writeInitialPlayerCoordinate(FILE* fp) const;
    void writeItems(FILE* fp) const;

    uint16_t width;
    uint16_t height;
    TileType defaultTileType;
    std::map<TileCoord, TileType> deviations;
    TileCoord playerCoordinate;
    TileCoord keyCoordinate;
    BoardModel* board;

};

#endif
