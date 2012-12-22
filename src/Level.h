#ifndef LEVEL_H
#define LEVEL_H

#include "KeyRunner.h"
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

	Tile* tile[GRID_HEIGHT][GRID_WIDTH];

	int level;

	Tile* tileHasPlayer;
	Tile* tileHasKey;

	bool parseLine(std::string line);

	bool playerHasKey;

	uint parseX;
	uint parseY;

	Animation* keyAnim;
	Animation* playerAnim;

	Tile* getMatchingTeleporterTile(Tile*);

public:
	Level();
	void load(int level);
	void draw();

	bool hasKey(int x, int y);
	bool hasPlayer(int x, int y);
	bool isWall(int x, int y);

	void movePlayer(Direction d);

	bool isComplete();
	int toInt();

	Tile* getTile(uint x, uint y) const;

	static unsigned int GetTotal();

};

#endif//LEVEL_H
