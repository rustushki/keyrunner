#include <stdexcept>
#include <sstream>
#include "../model/BoardModel.hpp"

/**
 * Constructor.
 * <p>
 * Initializes the tile type grid to be sized as getGridHeight() and getGridWidth() indicate.
 */
BoardModel::BoardModel() {
    for (int row = 0; row < getHeight(); row++) {
        std::vector<TileType> tileRow;
        for (int column = 0; column < getWidth(); column++) {
            tileRow.push_back(TileType::Empty);
        }
        this->tileType.push_back(tileRow);
    }
}

/**
 * Get the level number of the currently loaded level.
 * @return uint8_t
 */
uint8_t BoardModel::getLevelNum() const {
    return level;
}

/**
 * Set the number of the currently loaded level.
 * @param level
 */
void BoardModel::setLevelNum(uint8_t level) {
    this->level = level;
}

/**
 * Is the provided coordinate a wall?
 * @param coord
 * @return boolean
 */
bool BoardModel::isWall(TileCoord coord) const {
    return (tileType[coord.second][coord.first] == TileType::Wall);
}

/**
 * Get the coordinate of the key.
 * @return TileCoord
 */
TileCoord BoardModel::getKeyCoord() const {
    return keyCoord;
}

/**
 * Set the coordinate of the key.
 * @param tileCoord
 */
void BoardModel::setKeyCoord(TileCoord tileCoord) {
    keyCoord = tileCoord;
}

/**
 * Get the current tile of the player.
 * @return TileCoord
 */
TileCoord BoardModel::getPlayerCoord() const {
    return playerCoord;
}

/**
 * Assign a new coordinate to the player.
 * <p>
 * If the coordinate has the key, transfer ownership to the player. If the coordinate is a teleporter, move the player
 * to the matching tile.
 * @param tileCoord
 */
void BoardModel::setPlayerCoord(TileCoord tileCoord) {
    // Handle Teleporter Tiles and Empty Tiles
    if (isTeleporter(tileCoord)) {
        TileCoord matching = getMatchingTeleporterTileCoord(tileCoord);
        playerCoord = matching;
    } else {
        playerCoord = tileCoord;
    }
}

/**
 * Is the provided coordinate a teleporter?
 * @param TileCoord
 * @return boolean
 */
bool BoardModel::isTeleporter(TileCoord coord) const {
    TileType tt = tileType[coord.second][coord.first];

    return (   tt == TileType::TeleporterRed
            || tt == TileType::TeleporterGreen
            || tt == TileType::TeleporterBlue);
}

/**
 * Change the tile type of the tile at the provided tile coordinate.
 * @param coord
 * @param newTileType
 */
void BoardModel::changeTileType(TileCoord coord, TileType newTileType) {
    tileType[coord.second][coord.first] = newTileType;
}

/**
 * Get the type of the tile at the provided coordinate.
 * @param coord
 * @return TileType
 */
TileType BoardModel::getTileType(TileCoord coord) const {
    return tileType[coord.second][coord.first];
}

/**
 * Is the Tile at the provided TileCoord a Door?
 * @param coord
 * @return boolean
 */
bool BoardModel::isDoor(TileCoord coord) const {
    return tileType[coord.second][coord.first] == TileType::Door;
}

/**
 * Given a direction and a tile coordinate, return the tile coordinate to that direction.
 * @param coord
 * @param dir
 * @param TileCoord
 */
TileCoord BoardModel::getTileCoordInDirection(TileCoord coord, Direction dir) const {
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

/**
 * Get the tile coordinate above the provided tile coordinate
 * @param current
 * @return TileCoord
 */
TileCoord BoardModel::getTileCoordUp(TileCoord current) const {
    int x = current.first + 0;
    int y = current.second - 1;
    if (y < 0) {
        y = getHeight() - 1;
    }
    return TileCoord(x, y);
}

/**
 * Get the tile coordinate below the provided tile coordinate
 * @param current
 * @return TileCoord
 */
TileCoord BoardModel::getTileCoordDown(TileCoord current) const {
    int x = current.first + 0;
    int y = current.second + 1;
    if (y >= getHeight()) {
        y = 0;
    }
    return TileCoord(x, y);
}

/**
 * Get the tile coordinate left of the provided tile coordinate
 * @param current
 * @return TileCoord
 */
TileCoord BoardModel::getTileCoordLeft(TileCoord current) const {
    int x = current.first - 1;
    int y = current.second + 0;
    if (x < 0) {
        x = getWidth() - 1;
    }
    return TileCoord(x, y);
}

/**
 * Get the tile coordinate right of the provided tile coordinate
 * @param current
 * @return TileCoord
 */
TileCoord BoardModel::getTileCoordRight(TileCoord current) const {
    int x = current.first + 1;
    int y = current.second + 0;
    if (x >= getWidth()) {
        x = 0;
    }
    return TileCoord(x, y);
}

/**
 * Return true if the tile coordinate has a conveyor tile type.
 * @param coord
 * @return boolean
 */
bool BoardModel::isConveyor(TileCoord coord) const {
    TileType tt = tileType[coord.second][coord.first];

    return (   tt == TileType::ConveyorUp
            || tt == TileType::ConveyorDown
            || tt == TileType::ConveyorRight
            || tt == TileType::ConveyorLeft);
}

/**
 * Return the direction the conveyor is pointing towards.
 * @param coord
 * @return Direction
 * @throws std::invalid_argument if tile coordinate is not for conveyor tile
 */
Direction BoardModel::getConveyorDirection(TileCoord coord) const {
    TileType tt = tileType[coord.second][coord.first];
    if (tt == TileType::ConveyorUp) {
        return DIRECTION_UP;
    } else if (tt == TileType::ConveyorDown) {
        return DIRECTION_DOWN;
    } else if (tt == TileType::ConveyorRight) {
        return DIRECTION_RIGHT;
    } else if (tt == TileType::ConveyorLeft) {
        return DIRECTION_LEFT;
    }

    std::stringstream errorMessage;
    errorMessage << "Non-conveyor tile queried for direction.";
    throw std::invalid_argument(errorMessage.str());
}

/**
 * Return the 'drop off' spot of this tile if its a conveyor tile. Check each tile clock wise inclusive from the current
 * tile:
 * <ol>
 * <li>If the tile is a conveyor, return that tile.</li>
 * <li>If the tile is not wall or conveyor, make that tile the backup in case no conveyor is found. (second place)</li>
 * <li>If there is no suitable second place, return the current tile in the conveyor belt sequence</li>
 * </ol>
 */
TileCoord BoardModel::getNextConveyorTileCoord(TileCoord current) const {
    if (!isConveyor(current)) {
        std::stringstream errorMessage;
        errorMessage << "Trying to get next conveyor tile from non conveyor tile.";
        throw std::invalid_argument(errorMessage.str());
    }

    Direction direction = getConveyorDirection(current);
    Direction origDir = direction;

    // Initialize oppDir to pacify compiler
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
    TileCoord secondPlaceCoord(getWidth(), getHeight());
    TileCoord thirdPlaceCoord = current;

    while (true) {

        tryTileCoord = getTileCoordInDirection(current, direction);

        if (!isWall(tryTileCoord) && !isConveyor(tryTileCoord) && secondPlaceCoord.first != getWidth()) {
            secondPlaceCoord = tryTileCoord;

        // Prefer adjacent conveyor belt tiles.  Explicitly do not place the player on a conveyor belt tile in the
        // exact opposite direction of current travel
        } else if (isConveyor(tryTileCoord)) {
            if (direction != oppDir) {
                Direction newDirection = getConveyorDirection(tryTileCoord);

                TileCoord check = getTileCoordInDirection(tryTileCoord, newDirection);

                if (check.first != current.first || check.second != current.second) {
                    return tryTileCoord;
                }
            }
        }

        if (direction == DIRECTION_UP) {
            direction = DIRECTION_RIGHT;
        } else if (direction == DIRECTION_RIGHT) {
            direction = DIRECTION_DOWN;
        } else if (direction == DIRECTION_DOWN) {
            direction = DIRECTION_LEFT;
        } else if (direction == DIRECTION_LEFT) {
            direction = DIRECTION_UP;
        }

        if (direction == DIRECTION_COUNT) {
            direction = DIRECTION_UP;
        }

        if (direction == origDir) {
            break;
        }

    }

    if (secondPlaceCoord.first != getWidth() && secondPlaceCoord.second != getHeight()) {
        return secondPlaceCoord;
    }

    return thirdPlaceCoord;
}

/**
 * Given a teleporter tile X and Y, return the matching teleporter tile's X and Y.  Return as a vector int.
 * @param coord
 * @return TileCoord
 */
TileCoord BoardModel::getMatchingTeleporterTileCoord(TileCoord coord) const {

    TileCoord matching = coord;

    // Handle case where a non-teleporter tile is passed in.  Return the same
    // tile provided.  This should never happen.
    if (!isTeleporter(coord)) {
        matching = coord;


        // Normal case. Find the first matching teleporter tile.
    } else {

        // Search for the matching tile.
        bool found = false;
        for (uint16_t x = 0; x < getWidth(); x++) {
            for (uint16_t y = 0; y < getHeight(); y++) {

                if (x != coord.first || y != coord.second) {

                    // Found a TileCoord with a Teleporter Type of the same
                    // color which is not this tile.
                    if (tileType[coord.second][coord.first] == tileType[y][x]) {
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
            matching = coord;
        }
    }

    return matching;
}

/**
 * Return true if the provided tile coordinate has the key.
 * @param tileCoord
 * @return boolean
 */
bool BoardModel::tileCoordHasKey(TileCoord tileCoord) const {
    return (keyCoord == tileCoord);
}

/**
 * Return true if the provided tile coordinate has the player.
 * @param tileCoord
 * @return boolean
 */
bool BoardModel::tileCoordHasPlayer(TileCoord tileCoord) const {
    return (playerCoord == tileCoord);
}

/**
 * Get the Board Height.
 * @return uint16_t
 */
uint16_t BoardModel::getHeight() const {
    return 16;
}

/**
 * Get the Board Width.
 * @return uint16_t
 */
uint16_t BoardModel::getWidth() const {
    return 25;
}

