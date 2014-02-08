#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <vector>

#include "AnimationType.hpp"
#include "Direction.hpp"
#include "GridLayer.hpp"
#include "TileType.hpp"
#include "Tile.hpp"

class Animation;

class Level {

private:
    Level(int levelNum);

    void init();

    Tile* tile[GridLayer::GRID_HEIGHT][GridLayer::GRID_WIDTH];

    int level;

    Tile* tileHasPlayer;
    Tile* tileHasKey;

    bool playerHasKey;

    Animation* keyAnim;
    Animation* playerAnim;

    Tile* getMatchingTeleporterTile(Tile*);

    void buildConveyorAnimations();

public:
    ~Level();

    void refreshTiles() const;

    bool hasKey(int x, int y);
    bool hasPlayer(int x, int y);
    bool isWall(int x, int y);

    bool movePlayer(Direction d);
    bool movePlayerToTile(Tile*);
    Tile* getPlayerTile() const;
    Tile* getKeyTile() const;

    bool isComplete() const;
    int toInt() const;

    Tile* getTile(uint16_t x, uint16_t y) const;

    friend class LevelLoader;
    friend class LevelReader;

};

#endif//LEVEL_HPP
