#include "Animation.h"
#include "Level.h"
#include "TileType.h"
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
		if (fgets(line, sizeof(line), fp) == NULL) {
			break;
		}
		parseLine(std::string(line));
	}

	fclose(fp);
}

Tile* Level::getTile(uint x, uint y) const {
	if (y >= GRID_HEIGHT || x >= GRID_WIDTH) {
		return NULL;
	}
	return this->tile[y][x];
}

bool Level::parseLine(std::string line) {

	for (unsigned int n = 0; n < line.length(); n++) {

		char b = line[n];
		bool tileHasPlayer = false;
		bool tileHasKey    = false;

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
			tileHasKey = true;
			tt = TILETYPE_EMPTY;

		} else if (b == 'p') {
			tileHasPlayer = true;
			tt = TILETYPE_EMPTY;

		} else if (b == 't') {
			tt = TILETYPE_TELEPORTER_RED;

		} else if (b == 'u') {
			tt = TILETYPE_TELEPORTER_GREEN;

		} else if (b == 'w') {
			tt = TILETYPE_TELEPORTER_BLUE;

		} else if (b == '^') {
			tt = TILETYPE_CONVEY_UP;

		} else if (b == 'v') {
			tt = TILETYPE_CONVEY_DOWN;

		} else if (b == '<') {
			tt = TILETYPE_CONVEY_LEFT;

		} else if (b == '>') {
			tt = TILETYPE_CONVEY_RIGHT;

		} else {
			tt = (TileType)(b - 0x30);
		}

		Tile* tile = new Tile(tt, parseX, parseY, this);


		if (tileHasPlayer) {
			this->tileHasPlayer = tile;
		}

		if (tileHasKey) {
			this->tileHasKey = tile;
		}

		this->tile[parseY][parseX] = tile;
		parseX++;
	}

	return true;
}

void Level::draw() {

	SDL_mutexP(screenLock);

	// Blit all Tiles.
	for (int x = 0; x < GRID_WIDTH; x++) {

		for (int y = 0; y < GRID_HEIGHT; y++) {
			this->getTile(x, y)->draw();

		}
	}

	SDL_mutexV(screenLock);

}

bool Level::hasPlayer(int x, int y) {
	return (this->tileHasPlayer == this->getTile(x, y));
}

bool Level::hasKey(int x, int y) {
	return (this->tileHasKey == this->getTile(x, y));
}

void Level::movePlayer(Direction d) {
	
	if (d > DIRECTION_COUNT) {
		std::cout << "Invalid direction." << std::endl;
		exitGame();
	}

	Tile* newTile;
	if (d == DIRECTION_UP) {
		newTile = this->tileHasPlayer->up();
	}

	if (d == DIRECTION_DOWN) {
		newTile = this->tileHasPlayer->down();
	}

	if (d == DIRECTION_LEFT) {
		newTile = this->tileHasPlayer->left();
	}

	if (d == DIRECTION_RIGHT) {
		newTile = this->tileHasPlayer->right();
	}

	if (newTile != NULL && !newTile->isWall()) {
		Tile::AddChangedTile(this->tileHasPlayer);

		// New tile has the player.
		this->tileHasPlayer = newTile;

		// New location from movement to non-wall tile.
		Tile::AddChangedTile(this->tileHasPlayer);
	}

	// Handle Teleporter Tiles.
	if (this->tileHasPlayer->isTeleporter()) {

		std::vector<int> newPos;
		Tile* matching = this->getMatchingTeleporterTile(this->tileHasPlayer);

		this->tileHasPlayer = matching;

		// Redraw new location. 
		Tile::AddChangedTile(this->tileHasPlayer);
	}

	if (this->hasKey(this->tileHasPlayer->getX(), this->tileHasPlayer->getY())) {
		this->tileHasKey = NULL;
		this->playerHasKey = true;
	}

}

/* ------------------------------------------------------------------------------
 * getMatchingTeleporterTile - Given a teleporter tile X and Y, return the
 * matching teleporter tile's X and Y.  Return as a vector int.
 */
Tile* Level::getMatchingTeleporterTile(Tile* t) {

	Tile* matching = NULL;

	// Handle case where a non-telepoprter tile is passed in.  Return the same
	// tile provided.  This should never happen.
	if (!t->isTeleporter()) {
		matching = t;


	// Normal case. Find the first matching teleporter tile.
	} else {

		// Search for the matching tile.
		bool found = false;
		for (int x = 0; x < GRID_WIDTH; x++) {
			for (int y = 0; y < GRID_HEIGHT; y++) {

				if (x != t->getX() || y != t->getY()) {

					// Found the a Teleporter Tile of the same color which is not this tile.
					if (t->getType() == this->getTile(x,y)->getType()) {
						matching = this->getTile(x, y);
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
	return (this->playerHasKey && this->tileHasPlayer->isDoor());
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
