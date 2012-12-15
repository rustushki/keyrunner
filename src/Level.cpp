#include "Animation.h"
#include "Level.h"
#include "TileType.h"
#include <sstream>
#include <cstdio>

Level::Level() {
	this->playerHasKey = false;

	this->keyAnim    = Animation::AnimationFactory(ANIMATION_TYPE_KEY);
	this->playerAnim = Animation::AnimationFactory(ANIMATION_TYPE_PUMPKIN);
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
		if (fgets(line, sizeof(line), fp) == NULL) {
			break;
		}
		parseLine(std::string(line));
	}

	fclose(fp);
}

bool Level::parseLine(std::string line) {

	for (unsigned int n = 0; n < line.length(); n++) {

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

		this->tile[parseY][parseX] = new Tile(tt);
		parseX++;
	}

	return true;
}

void Level::draw() {

	SDL_mutexP(screenLock);

	// Blit all Tiles.
	for (int x = 0; x < GRID_WIDTH; x++) {

		for (int y = 0; y < GRID_HEIGHT; y++) {
			this->drawTile(x, y);

		}
	}

	SDL_mutexV(screenLock);

}

void Level::drawTile(int x, int y) {
	
	const uint tileSize = 25;

	// Determine the coordinate to draw the tile animation..
	uint xp = x*tileSize;
	uint yp = y*tileSize;

	Tile* tile = this->tile[y][x];
	tile->getAnimation()->advance(xp, yp);

	// Redraw the Key.
	if (this->hasKey(x, y)) {
		this->keyAnim->advance(xp, yp);
	}
	
	// Redraw the Player.
	if (this->hasPlayer(x, y)) {
		this->playerAnim->advance(xp, yp);
	}

}

bool Level::hasPlayer(int x, int y) {
	return (x == playerX && y == playerY);
}

bool Level::hasKey(int x, int y) {
	return (x == keyX && y == keyY);
}

bool Level::isWall(int x, int y) {
	return (this->tile[y][x]->getType() == TILETYPE_WALL);
}

bool Level::isDoor(int x, int y) {
	return (this->tile[y][x]->getType() == TILETYPE_DOOR);
}

/* ------------------------------------------------------------------------------
 * isTeleporterTile - Return true if the tile is a teleporter tile.
 */
bool Level::isTeleporterTile(int x, int y) {
	TileType tt = this->tile[y][x]->getType();

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

		// New location from movement to non-wall tile.
		this->addChangedTile(this->playerX, this->playerY);
	}

	// Stop referencing newPlayerX and newPlayerY.

	// Boundary wrap-around conditions.
	bool wrapAround = false;
	if (this->playerX < 0) {
		this->playerX = GRID_WIDTH-1;
		wrapAround = true;
	}

	if (this->playerY < 0) {
		this->playerY = GRID_HEIGHT-1;
		wrapAround = true;
	}

	if (this->playerX >= GRID_WIDTH) {
		this->playerX = 0;
		wrapAround = true;
	}

	if (this->playerY >= GRID_HEIGHT) {
		this->playerY = 0;
		wrapAround = true;
	}

	if (wrapAround) {
		// New location from wrapping around.
		this->addChangedTile(this->playerX, this->playerY);
	}

	// Handle Teleporter Tiles.
	if (this->isTeleporterTile(this->playerX, this->playerY)) {

		std::vector<int> newPos;
		newPos = this->getMatchingTeleporterTile(this->playerX, this->playerY);

		this->playerX = newPos[0];
		this->playerY = newPos[1];

		// Redraw new location. 
		this->addChangedTile(this->playerX, this->playerY);
	}

	if (this->hasKey(this->playerX, this->playerY)) {
		this->keyX = -1;
		this->keyY = -1;
		this->playerHasKey = true;
	}

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

unsigned int Level::GetTotal() {

	static int levelCount = -1;

	if (levelCount == -1) {
		int hiLevel = 10000;
		int loLevel =     1;
		int miLevel =     0;

		while (hiLevel >= loLevel) {

			miLevel = (hiLevel-loLevel)/2 + loLevel;

			std::stringstream ss;
			ss << LEVELPATH << miLevel;
			FILE* fp = fopen(ss.str().c_str(), "r");

			if (fp != NULL) {
				fclose(fp);
				loLevel = miLevel + 1;
			} else {
				hiLevel = miLevel - 1;
			}
		}
		levelCount = miLevel;
	}

	return levelCount;

}
