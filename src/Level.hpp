#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <vector>

#include "AnimationType.hpp"
#include "Direction.hpp"
#include "GridLayer.hpp"
#include "TileType.hpp"
#include "TileLayer.hpp"

class Animation;

class Level {

private:
    Level(int levelNum);

    void init();

    TileLayer* tile[GridLayer::GRID_HEIGHT][GridLayer::GRID_WIDTH];

    int level;

    TileLayer* tileHasPlayer;
    TileLayer* tileHasKey;

    bool playerHasKey;

    Animation* keyAnim;
    Animation* playerAnim;

    TileLayer* getMatchingTeleporterTile(TileLayer*);

    void buildConveyorAnimations();

public:
    ~Level();

    void refreshTiles() const;

    bool hasKey(int x, int y);
    bool hasPlayer(int x, int y);
    bool isWall(int x, int y);

    bool movePlayer(Direction d);
    bool movePlayerToTile(TileLayer*);
    TileLayer* getPlayerTile() const;
    TileLayer* getKeyTile() const;

    bool isComplete() const;
    int toInt() const;

    TileLayer* getTile(uint16_t x, uint16_t y) const;

    friend class LevelManager;

};

#endif//LEVEL_HPP
