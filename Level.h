#ifndef LEVEL_H
#define LEVEL_H

#include "KeyRunner.h"
#include <string>
#include <vector>

#define GRID_HEIGHT 16
#define GRID_WIDTH 25

enum TileType {

	  TILETYPE_EMPTY
	, TILETYPE_WALL
	, TILETYPE_DOOR
	, TILETYPE_TELEPORTER_RED
	, TILETYPE_TELEPORTER_GREEN
	, TILETYPE_TELEPORTER_BLUE
	, TILETYPE_COUNT

};

enum Direction {
	  DIRECTION_UP
	, DIRECTION_DOWN
	, DIRECTION_LEFT
	, DIRECTION_RIGHT
	, DIRECTION_COUNT
};

class Level {

private:

	TileType tile[GRID_HEIGHT][GRID_WIDTH];

	int level;

	int keyX;
	int keyY;

	int playerX;
	int playerY;

	bool parseLine(std::string line);

	bool playerHasKey;

	int parseX;
	int parseY;

	std::vector< std::vector<int> > changedTiles;
	void addChangedTile(int x, int y);

	std::vector<int> getMatchingTeleporterTile(int x, int y);

public:
	Level();
	void load(int level);
	void draw();
	void drawTile(int x, int y);
	void redrawChangedTiles();
	std::string getImgPath(std::string);
	SDL_Surface* getTileImage(TileType tt);
	SDL_Surface* getKeyImage();
	SDL_Surface* getPlayerImage();

	bool hasKey(int x, int y);
	bool hasPlayer(int x, int y);
	bool isWall(int x, int y);
	bool isDoor(int x, int y);
	bool isTeleporterTile(int x, int y);

	void movePlayer(Direction d);

	bool isComplete();
	int toInt();

};

#endif//LEVEL_H
