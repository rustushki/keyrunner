#ifndef BOARD_HPP
#define BOARD_HPP

#include <stdint.h>
#include <vector>
#include "../model/Direction.hpp"
#include "../model/TileType.hpp"
#include "../model/Model.hpp"

typedef std::pair<uint16_t, uint16_t> TileCoord;

class BoardModel : public Model {
public:
    BoardModel();
    bool isComplete() const;
    void setPlayerHasKey(bool playerHasKey);
    uint8_t getLevelNum() const;
    void setLevelNum(uint8_t level);
    bool isWall(TileCoord coord) const;
    TileCoord getKeyCoord() const;
    void setKeyCoord(TileCoord tileCoord);
    TileCoord getPlayerCoord() const;
    void setPlayerCoord(TileCoord tileCoord);
    bool isTeleporter(TileCoord coord) const;
    void changeTileType(TileCoord coord, TileType newTileType);
    TileType getTileType(TileCoord coord) const;
    bool isDoor(TileCoord coord) const;
    TileCoord getTileCoordInDirection(TileCoord coord, Direction d) const;
    TileCoord getTileCoordUp(TileCoord current) const;
    TileCoord getTileCoordDown(TileCoord current) const;
    TileCoord getTileCoordLeft(TileCoord current) const;
    TileCoord getTileCoordRight(TileCoord current) const;
    bool isConveyor(TileCoord coord) const;
    Direction getConveyorDirection(TileCoord coord) const;
    TileCoord getNextConveyorTileCoord(TileCoord tileCoord) const;
    TileCoord getMatchingTeleporterTileCoord(TileCoord coord) const;
    bool tileCoordHasKey(TileCoord tileCoord) const;
    bool tileCoordHasPlayer(TileCoord tileCoord) const;
    void movePlayerInDirection(Direction direction);
    uint16_t getHeight() const;
    uint16_t getWidth() const;

private:
    bool playerHasKey;
    uint8_t level;
    TileCoord keyCoord;
    TileCoord playerCoord;
    std::vector< std::vector<TileType> > tileType;
};

#endif
