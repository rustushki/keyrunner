#ifndef LEVEL_H
#define LEVEL_H

#include "TileType.h"
#include "AnimationType.h"
#include "Direction.h"
#include "Tile.h"
#include <string>
#include <vector>

class Animation;

#define GRID_HEIGHT 16
#define GRID_WIDTH 25

class Level {

private:
	Level(int levelNum);

	Tile* tile[GRID_HEIGHT][GRID_WIDTH];

	int level;

	Tile* tileHasPlayer;
	Tile* tileHasKey;

	bool playerHasKey;

	Animation* keyAnim;
	Animation* playerAnim;

	Tile* getMatchingTeleporterTile(Tile*);

	void buildConveyorAnimations();

public:
	void draw();

	bool hasKey(int x, int y);
	bool hasPlayer(int x, int y);
	bool isWall(int x, int y);

	bool movePlayer(Direction d);
	bool movePlayerToTile(Tile*);
	Tile* getPlayerTile() const;

	bool isComplete();
	int toInt();

	Tile* getTile(uint16_t x, uint16_t y) const;

	friend class LevelLoader;

};

#endif//LEVEL_H
