#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <vector>
#include "../model/Direction.hpp"
#include "../model/TileType.hpp"
#include "../model/Model.hpp"
#include "../model/State.hpp"

typedef std::pair<uint16_t, uint16_t> TileCoord;

class BoardModel : public Model {
public:
    BoardModel();

    State getState() const;
    void setState(State state);

    uint16_t getHeight() const;
    uint16_t getWidth() const;

    uint8_t getLevelNum() const;
    void setLevelNum(uint8_t level);

    TileCoord getKeyCoord() const;
    void setKeyCoord(TileCoord tileCoord);

    TileCoord getPlayerCoord() const;
    void setPlayerCoord(TileCoord tileCoord);

    bool isWall(TileCoord coord) const;
    bool isTeleporter(TileCoord coord) const;
    bool isDoor(TileCoord coord) const;
    bool isConveyor(TileCoord coord) const;

    TileType getTileType(TileCoord coord) const;
    void changeTileType(TileCoord coord, TileType newTileType);

    TileCoord getTileCoordInDirection(TileCoord coord, Direction d) const;
    TileCoord getTileCoordUp(TileCoord current) const;
    TileCoord getTileCoordDown(TileCoord current) const;
    TileCoord getTileCoordLeft(TileCoord current) const;
    TileCoord getTileCoordRight(TileCoord current) const;

    Direction getConveyorDirection(TileCoord coord) const;
    TileCoord getNextConveyorTileCoord(TileCoord tileCoord) const;

    TileCoord getMatchingTeleporterTileCoord(TileCoord coord) const;

    bool tileCoordHasKey(TileCoord tileCoord) const;
    bool tileCoordHasPlayer(TileCoord tileCoord) const;

private:
    uint8_t level;
    TileCoord keyCoord;
    TileCoord playerCoord;
    std::vector< std::vector<TileType> > tileType;
    State state;
};

#endif
