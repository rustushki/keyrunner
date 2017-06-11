#ifndef PLAY_MODEL_HPP
#define PLAY_MODEL_HPP

#include <utility>
#include <stdint.h>

#include "Direction.hpp"
#include "TileType.hpp"
#include "State.hpp"

typedef std::pair<uint16_t, uint16_t> TileCoord;

class PlayModel {

    public:
        // Fixed Height and Width of the Grid.
        static const uint16_t GRID_HEIGHT = 16;
        static const uint16_t GRID_WIDTH = 25;

        static PlayModel* GetInstance();
        bool isComplete() const;
        void setPlayerHasKey(bool playerHasKey);
        uint8_t getLevelNum() const;
        void setLevelNum(uint16_t level);
        bool isWall(TileCoord coord) const;
        bool isTeleporter(TileCoord coord) const;
        TileCoord getKeyCoord() const;
        void setKeyCoord(TileCoord tileCoord);
        TileCoord getPlayerCoord() const;
        void setPlayerCoord(TileCoord tileCoord);
        void changeTileType(TileCoord coord, TileType tt);
        TileType getTileType(TileCoord coord) const;
        bool isDoor(TileCoord coord) const;
        bool isConveyor(TileCoord coord) const;
        TileCoord getTileCoordInDirection(TileCoord coord, Direction d) const;
        Direction getConveyorDirection(TileCoord coord) const;
        TileCoord getNextConveyorTileCoord(TileCoord tileCoord) const;
        TileCoord getMatchingTeleporterTileCoord(TileCoord t) const;
        bool tileCoordHasKey(TileCoord tileCoord) const;
        bool tileCoordHasPlayer(TileCoord tileCoord) const;
        State getState() const;
        void setState(State state);
        long getTimeClock();
        void setTimeClock(uint16_t timeClockValue);
        void decrementTimeClock(uint16_t step);
        void incrementTimeClock(uint16_t step);

        TileCoord getTileCoordUp(TileCoord current) const;
        TileCoord getTileCoordDown(TileCoord current) const;
        TileCoord getTileCoordLeft(TileCoord current) const;
        TileCoord getTileCoordRight(TileCoord current) const;

    private:
        // PlayMode is singleton.
        static PlayModel* instance;
        PlayModel();

        bool playerHasKey;
        uint8_t level;
        State state;
        long timeClock;

        TileCoord keyCoord;
        TileCoord playerCoord;

        TileType tileType[PlayModel::GRID_HEIGHT][PlayModel::GRID_WIDTH];
};

#endif
