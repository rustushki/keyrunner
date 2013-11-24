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
	void parseVar(std::string line);

	bool playerHasKey;

	uint16_t parseX;
	uint16_t parseY;

	Animation* keyAnim;
	Animation* playerAnim;

	Tile* getMatchingTeleporterTile(Tile*);

	void buildConveyorAnimations();

public:
	Level();
	void load(int level);
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

	static uint16_t GetTotal();

};

#endif//LEVEL_H
