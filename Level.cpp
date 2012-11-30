#include "Level.h"
#include <sstream>
#include <cstdio>

Level::Level() {
	this->playerHasKey = false;
}

void Level::load(int level) {

	this->level = level;

	std::string levelFile = LEVELPATH;

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

		} else if (b == 't') {
			tt = TILETYPE_TELEPORTER_RED;

		} else if (b == 'u') {
			tt = TILETYPE_TELEPORTER_GREEN;

		} else if (b == 'v') {
			tt = TILETYPE_TELEPORTER_BLUE;

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


	// Are we currently flipping buffers?
	if (flipping) {
		
		// If so,
		// Wait until it's safe to blit to the screen.
		SDL_mutexP(flipLock);
		SDL_CondWait(blitCond, flipLock);

	}

	blitting = true;

	// Blit the Tile to the Screen.
	SDL_BlitSurface(surf, &srcRect, screen, &scrRect);

	if (this->hasKey(x, y)) {
		SDL_Surface* key = this->getKeyImage();
		SDL_BlitSurface(key, &srcRect, screen, &scrRect);
	} else if (this->hasPlayer(x, y)) {
		SDL_Surface* player = this->getPlayerImage();
		SDL_BlitSurface(player, &srcRect, screen, &scrRect);
	}

	blitting = false;

	// Signal that it's safe to flip the screen.
	SDL_mutexV(flipLock);
	SDL_CondSignal(flipCond);

}

std::string Level::getImgPath(std::string fn) {
	std::stringstream ss;
	ss << IMGPATH << fn;
	return ss.str();
}

SDL_Surface* Level::getTileImage(TileType type) {

	SDL_Surface* surf;
	if (type == TILETYPE_EMPTY) {
		static SDL_Surface* tile = IMG_Load(this->getImgPath("tile.png").c_str());
		return tile;

	} else if (type == TILETYPE_WALL) {
		static SDL_Surface* wall = IMG_Load(this->getImgPath("wall.png").c_str());
		return wall;

	} else if (type == TILETYPE_DOOR) {
		static SDL_Surface* door = IMG_Load(this->getImgPath("door.png").c_str());
		return door;

	} else if (type == TILETYPE_TELEPORTER_RED) {
		static SDL_Surface* tilered = IMG_Load(this->getImgPath("teleporter_red.png").c_str());
		return tilered;

	} else if (type == TILETYPE_TELEPORTER_GREEN) {
		static SDL_Surface* tilered = IMG_Load(this->getImgPath("teleporter_green.png").c_str());
		return tilered;

	} else if (type == TILETYPE_TELEPORTER_BLUE) {
		static SDL_Surface* tilered = IMG_Load(this->getImgPath("teleporter_blue.png").c_str());
		return tilered;

	} else {
		std::cout << "Invalid Tile Type. " << (int)type << std::endl;
		exitGame();
	}

	return surf;
}

SDL_Surface* Level::getKeyImage() {
	static SDL_Surface* key = IMG_Load(this->getImgPath("key.png").c_str());
	return key;
}

SDL_Surface* Level::getPlayerImage() {
	static SDL_Surface* pumpkin = IMG_Load(this->getImgPath("pumpkin.png").c_str());
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

/* ------------------------------------------------------------------------------
 * isTeleporterTile - Return true if the tile is a teleporter tile.
 */
bool Level::isTeleporterTile(int x, int y) {
	TileType tt = this->tile[y][x];

	return (    tt == TILETYPE_TELEPORTER_RED
	         || tt == TILETYPE_TELEPORTER_GREEN
	         || tt == TILETYPE_TELEPORTER_BLUE);
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
	}

	if (this->hasKey(newPlayerX, newPlayerY)) {
		this->addChangedTile(this->keyX, this->keyY);
		this->keyX = -1;
		this->keyY = -1;
		this->playerHasKey = true;
	}

	// Handle Teleporter Tiles.
	if (this->isTeleporterTile(newPlayerX, newPlayerY)) {

		std::vector<int> newPos;
		newPos = this->getMatchingTeleporterTile(newPlayerX, newPlayerY);

		this->playerX = newPos[0];
		this->playerY = newPos[1];
	}


	// Boundary wrap-around conditions.
	if (this->playerX < 0) {
		this->playerX = GRID_WIDTH-1;
	}

	if (this->playerY < 0) {
		this->playerY = GRID_HEIGHT-1;
	}

	if (this->playerX >= GRID_WIDTH) {
		this->playerX = 0;
	}

	if (this->playerY >= GRID_HEIGHT) {
		this->playerY = 0;
	}

	this->addChangedTile(this->playerX, this->playerY);

	this->redrawChangedTiles();

}

/* ------------------------------------------------------------------------------
 * getMatchingTeleporterTile - Given a teleporter tile X and Y, return the
 * matching teleporter tile's X and Y.  Return as a vector int.
 */
std::vector<int> Level::getMatchingTeleporterTile(int tileX, int tileY) {

	std::vector<int> matching;

	// Handle case where a non-telepoprter tile is passed in.  Return the same
	// tile provided.  This should never happen.
	if (!this->isTeleporterTile(tileX, tileY)) {
		matching.push_back(tileX);
		matching.push_back(tileY);


	// Normal case. Find the first matching teleporter tile.
	} else {

		// Search for the matching tile.
		bool found = false;
		for (int x = 0; x < GRID_WIDTH; x++) {
			for (int y = 0; y < GRID_HEIGHT; y++) {

				if (x != tileX || y != tileY) {

					// Found the a Teleporter Tile of the same color which is not this tile.
					if (tile[tileY][tileX] == tile[y][x]) {
						matching.push_back(x);
						matching.push_back(y);
						found = true;
						break;
					}

				}
			}
			if (found) {
				break;
			}
		}
	}

	return matching;


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
