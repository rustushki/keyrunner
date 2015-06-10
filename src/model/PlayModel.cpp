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
