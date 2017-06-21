#include <sstream>
#include "PlayModel.hpp"
#include "../controller/KeyRunner.hpp"

PlayModel* PlayModel::instance = NULL;
const uint16_t PlayModel::GRID_HEIGHT;
const uint16_t PlayModel::GRID_WIDTH;

PlayModel* PlayModel::GetInstance() {
    if (instance == NULL) {
        instance = new PlayModel();
    }

    return instance;
}

/**
 * Constructor.
 */
PlayModel::PlayModel() {
    // Do nothing.
}

/**
 * Is the currently loaded level finished?
 * <p>
 * This happens when the player has the key and is standing at the door.
 * @return boolean
 */
bool PlayModel::isComplete() const {
    return (playerHasKey && isDoor(getPlayerCoord()));
}

/**
 * Set if the player has the key.
 * @param playerHasKey
 */
void PlayModel::setPlayerHasKey(bool playerHasKey) {
    this->playerHasKey = playerHasKey;
    if (this->playerHasKey) {
        this->keyCoord = TileCoord(GRID_WIDTH, GRID_HEIGHT);
    }
}

/**
 * Get the level number of the currently loaded level.
 * @return uint8_t
 */
uint8_t PlayModel::getLevelNum() const {
    return level;
}

/**
 * Set the number of the currently loaded level.
 * @param level
 */
void PlayModel::setLevelNum(uint16_t level) {
    this->level = level;
}

bool PlayModel::isWall(TileCoord coord) const {
    return (tileType[coord.second][coord.first] == TILE_TYPE_WALL);
}

/* ------------------------------------------------------------------------------
 * Return the current tile of the key.
 */
TileCoord PlayModel::getKeyCoord() const {
    return keyCoord;
}

void PlayModel::setKeyCoord(TileCoord tileCoord) {
    keyCoord = tileCoord;
}

/* ------------------------------------------------------------------------------
 * Return the current tile of the player.
 */
TileCoord PlayModel::getPlayerCoord() const {
    return playerCoord;
}

/**
 * Assign a new coordinate to the player.
 * <p>
 * If the coordinate has the key, transfer ownership to the player. If the coordinate is a teleporter, move the player
 * to the matching tile.
 * @param tileCoord
 */
void PlayModel::setPlayerCoord(TileCoord tileCoord) {
    // Give the player the key if the tile has the key
    if (tileCoordHasKey(tileCoord)) {
        setPlayerHasKey(true);
    }

    // Handle Teleporter Tiles and Empty Tiles
    if (isTeleporter(tileCoord)) {
        TileCoord matching = getMatchingTeleporterTileCoord(tileCoord);
        playerCoord = matching;
    } else {
        playerCoord = tileCoord;
    }
}

/* ------------------------------------------------------------------------------
 * isTeleporter - Return true if the tile is a teleporter tile.
 */
bool PlayModel::isTeleporter(TileCoord coord) const {
    TileType tt = tileType[coord.second][coord.first];

    return (   tt == TILE_TYPE_TELEPORTER_RED
            || tt == TILE_TYPE_TELEPORTER_GREEN
            || tt == TILE_TYPE_TELEPORTER_BLUE);
}

void PlayModel::changeTileType(TileCoord coord, TileType tt) {
    tileType[coord.second][coord.first] = tt;
}

TileType PlayModel::getTileType(TileCoord coord) const {
    return tileType[coord.second][coord.first];
}

/* ------------------------------------------------------------------------------
 * Is the Tile at the provided TileCoord a Door?
 */
bool PlayModel::isDoor(TileCoord coord) const {
    return tileType[coord.second][coord.first] == TILE_TYPE_DOOR;
}

/* ------------------------------------------------------------------------------
 * Given a direction, return the tile to that direction from this tile.
 */
TileCoord PlayModel::getTileCoordInDirection(TileCoord coord, Direction dir) const {
    if (dir == DIRECTION_UP) {
        return getTileCoordUp(coord);
    } else if (dir == DIRECTION_DOWN) {
        return getTileCoordDown(coord);
    } else if (dir == DIRECTION_RIGHT) {
        return getTileCoordRight(coord);
    } else if (dir == DIRECTION_LEFT) {
        return getTileCoordLeft(coord);
    }

    return getTileCoordLeft(coord);
}

TileCoord PlayModel::getTileCoordUp(TileCoord current) const {
    int x = current.first + 0;
    int y = current.second - 1;
    if (y < 0) {
        y = GRID_HEIGHT-1;
    }
    return TileCoord(x, y);
}

TileCoord PlayModel::getTileCoordDown(TileCoord current) const {
    int x = current.first + 0;
    int y = current.second + 1;
    if (y >= GRID_HEIGHT) {
        y = 0;
    }
    return TileCoord(x, y);
}

TileCoord PlayModel::getTileCoordLeft(TileCoord current) const {
    int x = current.first - 1;
    int y = current.second + 0;
    if (x < 0) {
        x = GRID_WIDTH-1;
    }
    return TileCoord(x, y);
}

TileCoord PlayModel::getTileCoordRight(TileCoord current) const {
    int x = current.first + 1;
    int y = current.second + 0;
    if (x >= GRID_WIDTH) {
        x = 0;
    }
    return TileCoord(x, y);
}

/* ------------------------------------------------------------------------------
 * Return true if the TileCoord has a conveyor tile type.
 */
bool PlayModel::isConveyor(TileCoord coord) const {
    TileType tt = tileType[coord.second][coord.first];

    return (   tt == TILE_TYPE_CONVEY_UP
            || tt == TILE_TYPE_CONVEY_DOWN
            || tt == TILE_TYPE_CONVEY_RIGHT
            || tt == TILE_TYPE_CONVEY_LEFT);
}

/* ------------------------------------------------------------------------------
 * Return the direction the conveyor is pointing towards.
 */
Direction PlayModel::getConveyorDirection(TileCoord coord) const {
    TileType tt = tileType[coord.second][coord.first];
    if (tt == TILE_TYPE_CONVEY_UP) {
        return DIRECTION_UP;
    } else if (tt == TILE_TYPE_CONVEY_DOWN) {
        return DIRECTION_DOWN;
    } else if (tt == TILE_TYPE_CONVEY_RIGHT) {
        return DIRECTION_RIGHT;
    } else if (tt == TILE_TYPE_CONVEY_LEFT) {
        return DIRECTION_LEFT;
    }

    std::stringstream errorMessage;
    errorMessage << "Non-conveyor tile queried for direction.";
    throw std::invalid_argument(errorMessage.str());
}

/* ------------------------------------------------------------------------------
 *  Return the 'drop off' spot of this tile if its a conveyor tile.  The
 *  strategy here is relatively simple:
 *    Check each tile clock wise inclusive from the current tile:
 *        1. If the tile is a conveyor, return that tile.
 *
 *        2. If the tile is not wall or conveyor, make that tile the backup in
 *        case no conveyor is found. (second place)
 *
 *        3. If there is no suitable second place, return the current tile in
 *        the conveyor belt sequence.
 */
TileCoord PlayModel::getNextConveyorTileCoord(TileCoord current) const {
    if (!isConveyor(current)) {
        std::stringstream errorMessage;
        errorMessage << "Trying to get next conveyor tile from non conveyor tile.";
        throw std::invalid_argument(errorMessage.str());
    }

    Direction dir = getConveyorDirection(current);
    Direction origDir = dir;

    // Initialize oppDir to pacify compiler.
    Direction oppDir = DIRECTION_COUNT;

    if (origDir == DIRECTION_UP) {
        oppDir = DIRECTION_DOWN;
    } else if (origDir == DIRECTION_DOWN) {
        oppDir = DIRECTION_UP;
    } else if (origDir == DIRECTION_RIGHT) {
        oppDir = DIRECTION_LEFT;
    } else if (origDir == DIRECTION_LEFT) {
        oppDir = DIRECTION_RIGHT;
    }

    TileCoord tryTileCoord;
    TileCoord secondPlaceCoord(GRID_WIDTH, GRID_HEIGHT);
    TileCoord thirdPlaceCoord = current;

    while (true) {

        tryTileCoord = getTileCoordInDirection(current, dir);

        if (!isWall(tryTileCoord) && !isConveyor(tryTileCoord) && secondPlaceCoord.first != GRID_WIDTH) {
            secondPlaceCoord = tryTileCoord;

            // Prefer adjacent conveyor belt tiles.  Explicitly do not place the
            // player on a conveyor belt tile in the exact opposite direction of
            // current travel.
        } else if (isConveyor(tryTileCoord)) {

            if (dir != oppDir) {
                Direction dir = getConveyorDirection(tryTileCoord);

                TileCoord check = getTileCoordInDirection(tryTileCoord, dir);

                if (check.first != current.first || check.second != current.second) {
                    return tryTileCoord;
                }
            }
        }

        if (dir == DIRECTION_UP) {
            dir = DIRECTION_RIGHT;
        } else if (dir == DIRECTION_RIGHT) {
            dir = DIRECTION_DOWN;
        } else if (dir == DIRECTION_DOWN) {
            dir = DIRECTION_LEFT;
        } else if (dir == DIRECTION_LEFT) {
            dir = DIRECTION_UP;
        }

        if (dir == DIRECTION_COUNT) {
            dir = DIRECTION_UP;
        }

        if (dir == origDir) {
            break;
        }

    }

    if (secondPlaceCoord.first != GRID_WIDTH && secondPlaceCoord.second != GRID_HEIGHT) {
        return secondPlaceCoord;
    }

    return thirdPlaceCoord;
}

/* ------------------------------------------------------------------------------
 * Given a teleporter tile X and Y, return the matching teleporter tile's X and
 * Y.  Return as a vector int.
 */
TileCoord PlayModel::getMatchingTeleporterTileCoord(TileCoord t) const {

    TileCoord matching = t;

    // Handle case where a non-teleporter tile is passed in.  Return the same
    // tile provided.  This should never happen.
    if (!isTeleporter(t)) {
        matching = t;


        // Normal case. Find the first matching teleporter tile.
    } else {

        // Search for the matching tile.
        bool found = false;
        for (uint16_t x = 0; x < GRID_WIDTH; x++) {
            for (uint16_t y = 0; y < GRID_HEIGHT; y++) {

                if (x != t.first || y != t.second) {

                    // Found a TileCoord with a Teleporter Type of the same
                    // color which is not this tile.
                    if (tileType[t.second][t.first] == tileType[y][x]) {
                        matching = TileCoord(x, y);
                        found = true;
                        break;
                    }

                }
            }
            if (found) {
                break;
            }
        }

        // Handle case where there is no matching teleporter tile.
        if (!found) {
            matching = t;
        }
    }

    return matching;
}

bool PlayModel::tileCoordHasKey(TileCoord tileCoord) const {
    return (keyCoord == tileCoord);
}

/* ------------------------------------------------------------------------------
 * Determine whether this tile has the player.
 */
bool PlayModel::tileCoordHasPlayer(TileCoord tileCoord) const {
    return (playerCoord == tileCoord);
}

State PlayModel::getState() const {
    return state;
}

void PlayModel::setState(State state) {
    this->state = state;
}

long PlayModel::getTimeClock() {
    return timeClock;
}

void PlayModel::setTimeClock(uint16_t timeClockValue) {
    timeClock = timeClockValue;
}

void PlayModel::decrementTimeClock(uint16_t step) {
    timeClock -= step;
}

void PlayModel::incrementTimeClock(uint16_t step) {
    timeClock += step;
}

/**
 * Move the player in the provided direction.
 * @param direction
 */
void PlayModel::movePlayerInDirection(Direction direction) {
    TileCoord oldTileCoord = getPlayerCoord();
    TileCoord newTileCoord;
    if (direction == DIRECTION_UP) {
        newTileCoord = getTileCoordUp(oldTileCoord);
    }

    if (direction == DIRECTION_DOWN) {
        newTileCoord = getTileCoordDown(oldTileCoord);
    }

    if (direction == DIRECTION_LEFT) {
        newTileCoord = getTileCoordLeft(oldTileCoord);
    }

    if (direction == DIRECTION_RIGHT) {
        newTileCoord = getTileCoordRight(oldTileCoord);
    }

    // Do not move player if the new tile is a wall. Do not continue evaluating criteria either, such as teleporters
    // and wraparound. They do not apply since the player has attempt to walk into a wall
    if (isWall(newTileCoord)) {
        return;
    }

    // Move the player to the tile
    setPlayerCoord(newTileCoord);
}
