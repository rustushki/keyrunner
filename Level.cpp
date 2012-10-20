#include "Level.h"
#include <sstream>
#include <cstdio>

Level::Level() {
	this->playerHasKey = false;
}

void Level::load(int level) {

	this->level = level;

	std::string levelFile = "./level/";

	// Covert level integer into string.
	std::stringstream ss;
	ss << level;

	levelFile += ss.str();

	char line[255];

	const char* fn = levelFile.c_str();
	FILE* fp = fopen(fn, "r");

	this->parseX = 0;
	this->parseY = 0;

	while (!feof(fp)) {
		fgets(line, sizeof(line), fp);
		parseLine(std::string(line));
	}

	fclose(fp);
}

bool Level::parseLine(std::string line) {

	int n = 0;

	for (int n = 0; n < line.length(); n++) {

		char b = line[n];

		// Ignore whitespace in level files.
		if (b == '\t' || b == '\n' || b == '\r' || b == ' ') {
			continue;
		}

		if (parseX == GRID_WIDTH) {
			parseX = 0;
			parseY++;
		}
		
		if (parseY >= GRID_HEIGHT) {
			return false;
		}

		TileType tt;
		if (b == 'k') {
			this->keyX = parseX;
			this->keyY = parseY;
			tt = TILETYPE_EMPTY;
		} else if (b == 'p') {
			this->playerX = parseX;
			this->playerY = parseY;
			tt = TILETYPE_EMPTY;
		} else {
			tt = (TileType)(b - 0x30);
		}

		this->tile[parseY][parseX] = tt;
		parseX++;
	}

	return true;
}

void Level::draw() {

	// Blit all Tiles.
	for (int x = 0; x < GRID_WIDTH; x++) {

		for (int y = 0; y < GRID_HEIGHT; y++) {
			this->drawTile(x, y);

		}
	}

}

void Level::drawTile(int x, int y) {

	// X/Y Offset to center the tiles in the screen.
	static int xOffset = 0;
	static int yOffset = 0;

	SDL_Surface* surf = this->getTileImage(this->tile[y][x]);

	// Source Rect
	SDL_Rect srcRect;
	srcRect.h = surf->h;
	srcRect.w = surf->w;
	srcRect.x = 0;
	srcRect.y = 0;

	// Dest Rect
	SDL_Rect scrRect;
	scrRect.h = surf->h;
	scrRect.w = surf->w;

	scrRect.x = xOffset + x*srcRect.w;
	scrRect.y = yOffset + y*srcRect.h;

	// Blit the Tile to the Screen.
	SDL_CondWait(flipCond, flipLock);
	SDL_BlitSurface(surf, &srcRect, screen, &scrRect);
	SDL_CondSignal(flipCond);

	if (this->hasKey(x, y)) {
		SDL_Surface* key = this->getKeyImage();
		SDL_CondWait(flipCond, flipLock);
		SDL_BlitSurface(key, &srcRect, screen, &scrRect);
		SDL_CondSignal(flipCond);
	} else if (this->hasPlayer(x, y)) {
		SDL_Surface* player = this->getPlayerImage();
		SDL_CondWait(flipCond, flipLock);
		SDL_BlitSurface(player, &srcRect, screen, &scrRect);
		SDL_CondSignal(flipCond);
	}

}

SDL_Surface* Level::getTileImage(TileType type) {

	SDL_Surface* surf;
	if (type == TILETYPE_EMPTY) {
		static SDL_Surface* tile = IMG_Load("img/tile.png");
		return tile;
	} else if (type == TILETYPE_WALL) {
		static SDL_Surface* wall = IMG_Load("img/wall.png");
		return wall;
	} else if (type == TILETYPE_DOOR) {
		static SDL_Surface* door = IMG_Load("img/door.png");
		return door;
	} else {
		std::cout << "Invalid Tile Type. " << (int)type << std::endl;
		exitGame();
	}

	return surf;
}

SDL_Surface* Level::getKeyImage() {
	static SDL_Surface* key = IMG_Load("img/key.png");
	return key;
}

SDL_Surface* Level::getPlayerImage() {
	static SDL_Surface* pumpkin = IMG_Load("img/pumpkin.png");
	return pumpkin;
}

bool Level::hasPlayer(int x, int y) {
	return (x == playerX && y == playerY);
}

bool Level::hasKey(int x, int y) {
	return (x == keyX && y == keyY);
}

bool Level::isWall(int x, int y) {
	return (this->tile[y][x] == TILETYPE_WALL);
}

bool Level::isDoor(int x, int y) {
	return (this->tile[y][x] == TILETYPE_DOOR);
}

void Level::movePlayer(Direction d) {
	
	if (d > DIRECTION_COUNT) {
		std::cout << "Invalid direction." << std::endl;
		exitGame();
	}

	int newPlayerX = this->playerX;
	int newPlayerY = this->playerY;

	if (d == DIRECTION_UP) {
		newPlayerY--;
	}

	if (d == DIRECTION_DOWN) {
		newPlayerY++;
	}

	if (d == DIRECTION_LEFT) {
		newPlayerX--;
	}

	if (d == DIRECTION_RIGHT) {
		newPlayerX++;
	}

	if (!this->isWall(newPlayerX, newPlayerY)) {
		this->addChangedTile(this->playerX, this->playerY);

		this->playerX = newPlayerX;
		this->playerY = newPlayerY;

		this->addChangedTile(this->playerX, this->playerY);
	}

	if (this->hasKey(newPlayerX, newPlayerY)) {
		this->addChangedTile(this->keyX, this->keyY);
		this->keyX = -1;
		this->keyY = -1;
		this->playerHasKey = true;
	}

	this->redrawChangedTiles();

}

bool Level::isComplete() {
	return (this->playerHasKey && this->isDoor(this->playerX, this->playerY));
}

void Level::redrawChangedTiles() {

	while (!changedTiles.empty()) {

		// Get pair to update.
		std::vector<int> pair = this->changedTiles.back();
		int x = pair[0];
		int y = pair[1];

		
		// Redraw the tile referenced by that pair.
		this->drawTile(x, y);

		// Remove that pair from the changed tiles list.
		this->changedTiles.pop_back();
	}

}

void Level::addChangedTile(int x, int y) {
	std::vector<int> pair;
	pair.push_back(x);
	pair.push_back(y);
	this->changedTiles.push_back(pair);
}

int Level::toInt() {
	return this->level;
}
