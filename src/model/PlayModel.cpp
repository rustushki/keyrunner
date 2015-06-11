#include "PlayModel.hpp"

// TODO: Remove These:
#include "../gui/GridLayer.hpp"

PlayModel* PlayModel::instance = NULL;

PlayModel* PlayModel::GetInstance() {
    if (instance == NULL) {
        instance = new PlayModel();
    }

    return instance;
}

PlayModel::PlayModel() {
    // Do nothing.
}

/* ------------------------------------------------------------------------------
 * Is the current PlayModel is a Complete State?
 *
 * This happens when the player has the key and is standing at the door.
 */
bool PlayModel::isComplete() const {
    TileLayer* playerTile = GridLayer::GetInstance()->getPlayerTile();
    return (playerHasKey && isDoor(TileCoord(playerTile->getX(), playerTile->getY())));
}

void PlayModel::setPlayerHasKey(bool playerHasKey) {
    this->playerHasKey = playerHasKey;
}

uint16_t PlayModel::getLevelNum() const {
    return level;
}

void PlayModel::setLevelNum(uint16_t level) {
    this->level = level;
}

bool PlayModel::isWall(TileCoord coord) const {
    TileLayer* tile = GridLayer::GetInstance()->getTile(coord.first, coord.second);
    return (tile->getType() == TILETYPE_WALL);
}

/* ------------------------------------------------------------------------------
 * getKeyCoord - Return the current tile of the player.
 */
TileCoord PlayModel::getKeyCoord() const {
    return keyCoord;
}

/* ------------------------------------------------------------------------------
 * isTeleporter - Return true if the tile is a teleporter tile.
 */
bool PlayModel::isTeleporter(TileCoord coord) const {
    TileType tt = tileType[coord.second][coord.first];

    return (   tt == TILETYPE_TELEPORTER_RED
            || tt == TILETYPE_TELEPORTER_GREEN
            || tt == TILETYPE_TELEPORTER_BLUE);
}

void PlayModel::changeTileType(TileCoord coord, TileType tt) {
    tileType[coord.second][coord.first] = tt;
}

/* ------------------------------------------------------------------------------
 * Is the Tile at the provided TileCoord a Door?
 */
bool PlayModel::isDoor(TileCoord coord) const {
    return tileType[coord.second][coord.first] == TILETYPE_DOOR;
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
 * Return true if the TileLayer is a conveyor tile.
 */
bool PlayModel::isConveyor(TileCoord coord) const {
    TileType tt = tileType[coord.second][coord.first];

    return (   tt == TILETYPE_CONVEY_UP
            || tt == TILETYPE_CONVEY_DOWN
            || tt == TILETYPE_CONVEY_RIGHT
            || tt == TILETYPE_CONVEY_LEFT);
}


