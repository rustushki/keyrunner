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

	int keyX;
	int keyY;

	int playerX;
	int playerY;

	bool parseLine(std::string line);

	bool playerHasKey;

	int parseX;
	int parseY;

	Animation* keyAnim;
	Animation* playerAnim;

	std::vector< std::vector<int> > changedTiles;
	void addChangedTile(int x, int y);

	std::vector<int> getMatchingTeleporterTile(int x, int y);

public:
	Level();
	void load(int level);
	void draw();
	void drawTile(int x, int y);
	void redrawChangedTiles();

	bool hasKey(int x, int y);
	bool hasPlayer(int x, int y);
	bool isWall(int x, int y);
	bool isDoor(int x, int y);
	bool isTeleporterTile(int x, int y);

	void movePlayer(Direction d);

	bool isComplete();
	int toInt();

	static unsigned int GetTotal();

};

#endif//LEVEL_H
