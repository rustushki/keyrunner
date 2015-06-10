#ifndef PLAYMODEL_HPP
#define PLAYMODEL_HPP

#include <utility>
#include <stdint.h>

typedef std::pair<uint16_t, uint16_t> TileCoord;

class PlayModel {

    public:
        static PlayModel* GetInstance();
        bool isComplete() const;
        void setPlayerHasKey(bool playerHasKey);
        uint16_t getLevelNum() const;
        void setLevelNum(uint16_t level);
        bool isWall(TileCoord coord) const;
        TileCoord getKeyCoord() const;

    private:
        // PlayMode is singleton.
        static PlayModel* instance;
        PlayModel();

        bool playerHasKey;
        uint16_t level;

        TileCoord keyCoord;
};

#endif//PLAYMODEL_HPP
