#ifndef PLAYMODEL_HPP
#define PLAYMODEL_HPP

#include <utility>
#include <stdint.h>

#include "../game/TileType.hpp"

typedef std::pair<uint16_t, uint16_t> TileCoord;

class PlayModel {

    public:
        // Fixed Height and Width of the Grid.
        static const uint16_t GRID_HEIGHT = 16;
        static const uint16_t GRID_WIDTH  = 25;

        static PlayModel* GetInstance();
        bool isComplete() const;
        void setPlayerHasKey(bool playerHasKey);
        uint16_t getLevelNum() const;
        void setLevelNum(uint16_t level);
        bool isWall(TileCoord coord) const;
        bool isTeleporter(TileCoord coord) const;
        TileCoord getKeyCoord() const;
        void changeTileType(TileCoord coord, TileType tt);

    private:
        // PlayMode is singleton.
        static PlayModel* instance;
        PlayModel();

        bool playerHasKey;
        uint16_t level;

        TileCoord keyCoord;

        TileType tileType[GRID_HEIGHT][GRID_WIDTH];
};

#endif//PLAYMODEL_HPP
