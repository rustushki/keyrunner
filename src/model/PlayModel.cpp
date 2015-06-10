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

bool PlayModel::isComplete() const {
    return (playerHasKey && GridLayer::GetInstance()->getPlayerTile()->isDoor());
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
