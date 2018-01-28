#ifndef BINARY_LEVEL_MANAGER_HPP
#define BINARY_LEVEL_MANAGER_HPP

#include <string>
#include <map>
#include <vector>

#include "../model/BoardModel.hpp"
#include "../model/LevelManager.hpp"
#include "../model/TileType.hpp"

class Level;

class BinaryLevelManager : public LevelManager {
public:
    explicit BinaryLevelManager();
    uint8_t getLevelCount() const override;
    bool levelExists(uint8_t levelNumber) const override;
    void read(BoardModel* boardModel) override;
    void write(const BoardModel* boardModel) const override;
    void create(BoardModel* boardModel) override;

private:
    std::string getPath(uint8_t levelNumber, bool inCurrentWorkingDirectory) const;
    void resetLevelManager(BoardModel* boardModel);
    void populateBoard(BoardModel* boardModel);

    void readSize(FILE* fp);
    void readDefaultTileType(FILE* fp);
    void readDeviations(FILE* fp);
    void readEntities(FILE* fp);

    void writeSize(FILE* fp, const BoardModel* boardModel) const;
    void writeDefaultTileType(FILE* fp) const;
    void writeDeviations(FILE* fp, const BoardModel* boardModel) const;
    void writeEntities(FILE* fp, const BoardModel* boardModel) const;

    uint16_t width;
    uint16_t height;
    TileType defaultTileType;
    std::map<TileCoordinate, TileType> deviations;
    Coordinate playerCoordinate{0, 0};
    Coordinate keyCoordinate{0, 0};
    std::vector<BoardEntity*> entities;

};

#endif
