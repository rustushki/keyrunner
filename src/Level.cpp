#include "Animation.h"
#include "ConveyorAnimation.h"
#include "Level.h"
#include "TileType.h"
#include <sstream>
#include <cstdio>

Level::Level() {
	this->playerHasKey = false;
	this->tileHasPlayer = NULL;
	this->tileHasKey = NULL;
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

	// 0 - parse grid lines
	// 1 - parse variables
	int parseState = 0;

	while (!feof(fp)) {
		if (fgets(line, sizeof(line), fp) == NULL) {
			break;
		}

		if (parseState == 0) {
			if (!this->parseLine(std::string(line))) {
				parseState = 1;
			}
		}
		
		if (parseState == 1) {
			this->parseVar(std::string(line));
		}

	}

	fclose(fp);
	
	this->buildConveyorAnimations();

}

bool Level::parseLine(std::string line) {

	for (unsigned int n = 0; n < line.length(); n++) {

		char b = line[n];
		bool tileHasPlayer = false;
		bool tileHasKey    = false;

		// Ignore these whitespace chars in level files when parsing tiles.
		if (b == '\t' || b == '\n' || b == '\r') {
			continue;
		}

		if (b == ';') {
			break;
		}

		if (parseX == GRID_WIDTH) {
			parseX = 0;
			parseY++;
		}
		
		if (parseY >= GRID_HEIGHT) {
			return false;
		}

		TileType tt = TILETYPE_EMPTY;
		if (b == 't') {
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

		} else if (b == ' ') {
			tt = TILETYPE_EMPTY;

		} else if (b == 'd') {
			tt = TILETYPE_DOOR;

		} else if (b == '1') {
			tt = TILETYPE_WALL;

		} else {
			std::cout << "Invalid tiletype in level file: " << b << std::endl;
			std::cout << "x,y = " << parseX << "," << parseY << std::endl;
			exitGame();
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

/* ------------------------------------------------------------------------------
 * parseVar - Parse variable definitions in level files.
 *
 * k  x,y   // Location of key
 * p  x,y   // Location of player
 *
 * TODO:
 *     c1 s     // Speed of Conveyor 1
 *     c2 n     // Speed of Conveyor 2
 *     Conveyors may be identified in their order from the top left of the
 *     screen.  For example, the first conveyor from the top left is c1.
 *
 * Expected Value Types (may use whitespace):
 *
 *  x,y       // pair of integers
 *  x, y      // pair of integers
 *  x	,   y // still a pair of integers
 *
 * Errors:
 *   1. Nonexistent conveyor
 *   2. More than 1 key
 *   3. More than 1 player
 *   4. Illegal value type
 */
void Level::parseVar(std::string line) {

	// Some flags to tell the parser what to look for.
	bool lookPlayerCoord = false;
	bool lookKeyCoord    = false;

	// Iterate over each char in the line.
	for (unsigned int n = 0; n < line.length(); n++) {

		char b = line[n];

		// Ignore whitespace in level files.
		if (b == '\t' || b == '\n' || b == '\r' || b == ' ') {
			continue;
		}

		// Everything after comment char is ignored.
		if (b == ';') {
			break;
		}

		// Level wants to tell us where the player should be placed.
		if (b == 'p') {
			lookPlayerCoord = true;
			if (this->tileHasPlayer != NULL) {
				std::cout << "More than 1 Player location specified in Level file." << std::endl;
				exitGame();
			}
			continue;

		// Level wants to tell us where the key should be placed.
		} else if (b == 'k') {
			lookKeyCoord = true;
			if (this->tileHasKey != NULL) {
				std::cout << "More than 1 key location specified in Level file." << std::endl;
				exitGame();
			}
			continue;

		// We're parsing a coordinate pair for either the key or the player.
		} else if (lookPlayerCoord || lookKeyCoord) {

			// Initialize the pair values.
			int x = -1;
			int y = -1;

			// We'll read each numeric character into the stream.
			std::stringstream s;

			// Continue until the end of the line.
			while (n < line.length()) {

				// If a comma is encountered, expect that we now have the X
				// value.
				if (b == ',') {
					
					// Convert the collected data into an integer and flush the
					// stream.
					x = atoi(s.str().c_str());
					s.str("");

				// If EOL is encountered, expect that we now have the Y value.
				} else if (n == line.length()-1) {

					// Convert stringstream contents to int and break out of
					// this coordinate parser.
					y = atoi(s.str().c_str());
					break;

				} else if (b == '\t' || b == '\n' || b == '\r' || b == ' ') {
					// next char

				// Unexpected character encountered.
				} else if (!isdigit(b)) {
					std::cout << "Illegal value in expected pair in level file" << std::endl;

				// X value has not yet been read.  Read in the next character.
				} else if (x == -1) {
					s << b;

				// Y value has not yet been read.  Read in the next character.
				} else if (y == -1) {
					s << b;

				// Otherwise the level file is badly formatted.
				} else {
					std::cout << "Coordinate parser in bad state." << std::endl;
					std::cout << "Check Level File Format." << std::endl;
					exitGame();
				}

				b = line[++n];
			}

			Tile* tile = this->getTile(x-1, y-1);
			if (lookKeyCoord) {
				this->tileHasKey = tile;
			} else if (lookPlayerCoord) {
				this->tileHasPlayer = tile;
			}

			lookKeyCoord    = false;
			lookPlayerCoord = false;
		} else {
			std::cout << "Illegal value in level file: '" << b << "'" << std::endl;
			exitGame();
		}

	}
}

void Level::buildConveyorAnimations() {

	ConveyorAnimation::ClearConveyors();

	for (int x = 0; x < GRID_WIDTH; x++) {

		for (int y = 0; y < GRID_HEIGHT; y++) {
			Tile* tile = this->getTile(x, y);

			if (tile->isConveyor()) {

				if (!ConveyorAnimation::TileInConveyor(tile)) {

					// This Tile must be part of a Conveyor.  Find the start of
					// the Conveyor.

					Tile* q = tile;
					Tile* p = q;

					if (ConveyorAnimation::TileInConveyor(p)) {
						continue;
					}

					//std::cout << "Found Tile: " << q->getX() << "," << q->getY() << std::endl;

					Direction conveyDir = q->getConveyorDirection();

					// Initialize oppDir to something invalid to pacify
					// compiler warnings.
					Direction oppDir = DIRECTION_COUNT;

					// Determine the opposite direction of this belt.
					if (conveyDir == DIRECTION_UP) {
						oppDir = DIRECTION_DOWN;
					} else if (conveyDir == DIRECTION_DOWN) {
						oppDir = DIRECTION_UP;
					} else if (conveyDir == DIRECTION_LEFT) {
						oppDir = DIRECTION_RIGHT;
					} else if (conveyDir == DIRECTION_RIGHT) {
						oppDir = DIRECTION_LEFT;
					}

					//std::cout << "\tOppDir: " << oppDir << std::endl;


					// A circular conveyor is one which stretches from one
					// border to the other, causing it it wrap-around.  We need
					// to check for this behavior so that we can prevent
					// infinite looping here.
					bool circular = false;

					// Go backwards along the belt until you hit a non-conveyor
					// tile, or a conveyor tile which is part of another
					// conveyor, or we find that the current belt is circular.
					Tile* prev = p;
					while (    p->isConveyor()
					        && p->getConveyorDirection() == conveyDir
							&& !ConveyorAnimation::TileInConveyor(p)
							&& !circular) {

						prev = p;

						if (oppDir == DIRECTION_UP) {
							p = p->up();
						} else if (oppDir == DIRECTION_DOWN) {
							p = p->down();
						} else if (oppDir == DIRECTION_RIGHT) {
							p = p->right();
						} else if (oppDir == DIRECTION_LEFT) {
							p = p->left();
						}

						if (p == q) {
							circular = true;
						}

					}

					Tile* start = p = prev;
					std::vector<Tile*> conveyorTiles;

					//std::cout << "\tStart " << start->getX() << "," << start->getY() << std::endl;
					//std::cout << "\t" << p->isConveyor() << std::endl;
					//std::cout << "\t" << p->getConveyorDirection() << "=" << conveyDir << std::endl;
					//std::cout << "\t" << ConveyorAnimation::TileInConveyor(p) << std::endl;

					int tileNum = 0;
					// Now follow the conveyor from its start until a
					// non-conveyor tile, or a conveyor tile which is part of
					// another conveyor or if the belt is found to be circular,
					// the start tile.
					while (    p->isConveyor()
					        && p->getConveyorDirection() == conveyDir
							&& !ConveyorAnimation::TileInConveyor(p)) {

						conveyorTiles.push_back(p);

						tileNum++;
						//std::cout << "\ttile #" << tileNum << " " << p->getX() << "," << p->getY() << std::endl;

						if (conveyDir == DIRECTION_UP) {
							p = p->up();
						} else if (conveyDir == DIRECTION_DOWN) {
							p = p->down();
						} else if (conveyDir == DIRECTION_RIGHT) {
							p = p->right();
						} else if (conveyDir == DIRECTION_LEFT) {
							p = p->left();
						}

						if (circular) {
							if (p == start) {
								break;
							}
						}

					}

					// Creating a ConveyorAnimation causes it to be added to
					// the ConveyorAnimation array.
					new ConveyorAnimation(conveyorTiles);

				}

			}
		}

	}
	
}

Tile* Level::getTile(uint x, uint y) const {
	if (y >= GRID_HEIGHT || x >= GRID_WIDTH) {
		return NULL;
	}
	return this->tile[y][x];
}

void Level::draw() {

	SDL_LockMutex(screenLock);

	// Blit all Tiles.
	for (int x = 0; x < GRID_WIDTH; x++) {

		for (int y = 0; y < GRID_HEIGHT; y++) {
			this->getTile(x, y)->draw();

		}
	}

	SDL_UnlockMutex(screenLock);

}

bool Level::hasPlayer(int x, int y) {
	return (this->tileHasPlayer == this->getTile(x, y));
}

bool Level::hasKey(int x, int y) {
	return (this->tileHasKey == this->getTile(x, y));
}

/* ------------------------------------------------------------------------------
 * movePlayer - Move the player in the provided direction by one tile.  Handle
 * walls, teleporters, keys and other gameplay elements.
 */
bool Level::movePlayer(Direction d) {
	
	SDL_LockMutex(levelLoadLock);

	if (d > DIRECTION_COUNT) {
		std::cout << "Invalid direction." << std::endl;
		exitGame();
	}

	Tile* newTile = NULL;
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

	bool interruptMovement = this->movePlayerToTile(newTile);
	SDL_UnlockMutex(levelLoadLock);

	return interruptMovement;
}

/* ------------------------------------------------------------------------------
 * movePlayerToTile - Given a tile, move a player to that tile.  Refuse to move
 * a player to a wall tile.  If the tile has the key, the player will take it.
 * If the tile is a lock/door and the player has the key then end the level.
 * If the tile is a teleporter, then actually move the player to the matching
 * tile. 
 *
 * Return true if the player movement is to be interrupted.  It will do so if
 * the provided tile is a wall or a teleporter.
 */
bool Level::movePlayerToTile(Tile* newTile) {
	if (newTile == NULL) {

		return true;
	}
		
	// Do not move player if the new tile is a wall.  Do not continue
	// evaluating criteria either, such as teleporters and wraparound.  They do
	// not apply since the player has attempt to walk into a wall.
	if (newTile->isWall()) {
		return true;
	}

	// Move the player to the tile.
	Tile::AddChangedTile(this->tileHasPlayer);
	this->tileHasPlayer = newTile;
	Tile::AddChangedTile(this->tileHasPlayer);

	// Give the player the key if the tile has the key.
	if (this->hasKey(this->tileHasPlayer->getX(), this->tileHasPlayer->getY())) {
		this->tileHasKey = NULL;
		this->playerHasKey = true;
	}

	// Handle Teleporter Tiles.
	if (this->tileHasPlayer->isTeleporter()) {
		Tile* matching = this->getMatchingTeleporterTile(this->tileHasPlayer);
		this->tileHasPlayer = matching;
		Tile::AddChangedTile(this->tileHasPlayer);
		return true;
	}

	// Interrupt movement if the level is over due to prior movement.
	if (this->isComplete()) {
		return true;
	}

	// Normal case, movement is not interrupted.
	return false;
}

/* ------------------------------------------------------------------------------
 * getPlayerTile - Return the current tile of the player.
 */
Tile* Level::getPlayerTile() const {
	return this->tileHasPlayer;
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
		for (Uint16 x = 0; x < GRID_WIDTH; x++) {
			for (Uint16 y = 0; y < GRID_HEIGHT; y++) {

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
