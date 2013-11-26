#include <sstream>

#include "KeyRunner.h"
#include "LevelLoader.h"

Level* LevelLoader::Load(int levelNum) {

	Level* level = new Level(levelNum);

	// Build path to Level File.
	std::string levelFile = LEVELPATH;
	std::stringstream ss;
	ss << levelNum;
	levelFile += ss.str();

	char line[255];

	const char* fn = levelFile.c_str();
	FILE* fp = fopen(fn, "r");

	int parseX = 0;
	int parseY = 0;

	// 0 - parse grid lines
	// 1 - parse variables
	int parseState = 0;

	while (!feof(fp)) {
		if (fgets(line, sizeof(line), fp) == NULL) {
			break;
		}

		if (parseState == 0) {
			if (!ParseLine(level, parseX, parseY, std::string(line))) {
				parseState = 1;
			}
		}
		
		if (parseState == 1) {
			ParseVar(level, std::string(line));
		}

	}

	fclose(fp);

	level->buildConveyorAnimations();

	return level;
}

bool LevelLoader::ParseLine(Level* level, int& parseX, int& parseY, std::string line) {

	for (uint16_t n = 0; n < line.length(); n++) {

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
			KeyRunner::exitGame();
		}

		Tile* tile = new Tile(tt, parseX, parseY, level);


		if (tileHasPlayer) {
			level->tileHasPlayer = tile;
		}

		if (tileHasKey) {
			level->tileHasKey = tile;
		}

		level->tile[parseY][parseX] = tile;
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
void LevelLoader::ParseVar(Level* level, std::string line) {

	// Some flags to tell the parser what to look for.
	bool lookPlayerCoord = false;
	bool lookKeyCoord    = false;

	// Iterate over each char in the line.
	for (uint16_t n = 0; n < line.length(); n++) {

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
			if (level->tileHasPlayer != NULL) {
				std::cout << "More than 1 Player location specified in Level file." << std::endl;
				KeyRunner::exitGame();
			}
			continue;

		// Level wants to tell us where the key should be placed.
		} else if (b == 'k') {
			lookKeyCoord = true;
			if (level->tileHasKey != NULL) {
				std::cout << "More than 1 key location specified in Level file." << std::endl;
				KeyRunner::exitGame();
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
					KeyRunner::exitGame();
				}

				b = line[++n];
			}

			Tile* tile = level->getTile(x-1, y-1);
			if (lookKeyCoord) {
				level->tileHasKey = tile;
			} else if (lookPlayerCoord) {
				level->tileHasPlayer = tile;
			}

			lookKeyCoord    = false;
			lookPlayerCoord = false;
		} else {
			std::cout << "Illegal value in level file: '" << b << "'" << std::endl;
			KeyRunner::exitGame();
		}

	}
}

uint16_t LevelLoader::GetTotal() {

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

