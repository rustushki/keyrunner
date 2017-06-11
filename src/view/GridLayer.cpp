#include <iostream>
#include <sstream>

#include "ConveyorAnimation.hpp"
#include "GridLayer.hpp"
#include "../controller/KeyRunner.hpp"

GridLayer* GridLayer::instance = NULL;

GridLayer* GridLayer::GetInstance() {
    if (instance == NULL) {
        instance = new GridLayer();
    }

    return instance;
}

GridLayer::GridLayer() {
    for (uint16_t x = 0; x < PlayModel::GRID_WIDTH; x++) {
        for (uint16_t y = 0; y < PlayModel::GRID_HEIGHT; y++) {
            tile[y][x] = new TileLayer(TILE_TYPE_EMPTY, x, y);
        }
    }
}

void GridLayer::init() {
    clearAnimatedTiles();
    ConveyorAnimation::BuildConveyorAnimations();
}

/* ------------------------------------------------------------------------------
 * PushAnimatedTile - Push the provided animated tile onto the vector of tiles
 * which have animations which require advancing.
 */
void GridLayer::pushAnimatedTile(TileLayer* tile) {
    animatedTiles.push_back(tile);
}

/* ------------------------------------------------------------------------------
 * ClearAnimatedTiles - Empty the vector containing Tiles which have Animations
 * which would otherwise be rendered on screen updates.
 */
void GridLayer::clearAnimatedTiles() {
    animatedTiles.clear();
}

/**
 * Iterate the list of animations which require advancing and advance each one.
 */
void GridLayer::animateTiles() {
    for (uint16_t x = 0; x < animatedTiles.size(); x++) {
        TileLayer* tile = animatedTiles[x];
        if (tile->getAnimation()->isAnimating()) {
            tile->getAnimation()->advance();
        }
    }
}

void GridLayer::draw(SDL_Renderer* renderer) {
    // Redraw the tile referenced by that pair.
    for (int y = 0; y < PlayModel::GRID_HEIGHT; y++) {
        for (int x = 0; x < PlayModel::GRID_WIDTH; x++) {
            tile[y][x]->draw(renderer);
        }
    }
}

SDL_Rect GridLayer::getRect() const {
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = PlayModel::GRID_WIDTH * TileLayer::SIZE;
    r.h = PlayModel::GRID_HEIGHT * TileLayer::SIZE;
    return r;
}

/**
 * Update any data in the GridLayer which affects the pending display of the GridLayer.
 */
void GridLayer::update() {
    // Start any remaining conveyor tiles.
    ConveyorAnimation::StartConveyors();

    // Advance any animated tiles.
    animateTiles();
}

GridLayer::~GridLayer() {
    for (int x = 0; x < PlayModel::GRID_WIDTH; x++) {
        for (int y = 0; y < PlayModel::GRID_HEIGHT; y++) {
            delete tile[y][x];
        }
    }
}

TileLayer* GridLayer::getTile(uint16_t x, uint16_t y) const {
    if (y >= PlayModel::GRID_HEIGHT || x >= PlayModel::GRID_WIDTH) {
        return NULL;
    }
    return tile[y][x];
}

void GridLayer::changeTileType(uint16_t x, uint16_t y, TileType tt) {
    TileLayer* tile = getTile(x, y);
    if (tile->getType() != tt) {
        tile->setType(tt);
    }
}

/* ------------------------------------------------------------------------------
 * movePlayer - Move the player in the provided direction by one tile.  Handle
 * walls, teleporters, keys and other gameplay elements.
 */
bool GridLayer::movePlayer(Direction d) {

    if (d > DIRECTION_COUNT) {
        std::stringstream errorMessage;
        errorMessage << "Invalid direction passed to movePlayer (" << d << ")";
        throw std::invalid_argument(errorMessage.str());
    }

    PlayModel* playModel = PlayModel::GetInstance();
    TileCoord oldTileCoord = playModel->getPlayerCoord();
    TileCoord newTileCoord;
    if (d == DIRECTION_UP) {
        newTileCoord = playModel->getTileCoordUp(oldTileCoord);
    }

    if (d == DIRECTION_DOWN) {
        newTileCoord = playModel->getTileCoordDown(oldTileCoord);
    }

    if (d == DIRECTION_LEFT) {
        newTileCoord = playModel->getTileCoordLeft(oldTileCoord);
    }

    if (d == DIRECTION_RIGHT) {
        newTileCoord = playModel->getTileCoordRight(oldTileCoord);
    }

    TileLayer* newTile = getTile(newTileCoord.first, newTileCoord.second);

    // Will interrupt movement if it returns true
    return movePlayerToTile(newTile);
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
bool GridLayer::movePlayerToTile(TileLayer* newTile) {
    PlayModel* playModel = PlayModel::GetInstance();

    if (newTile == NULL) {
        return true;
    }

    // Do not move player if the new tile is a wall.  Do not continue
    // evaluating criteria either, such as teleporters and wraparound.  They do
    // not apply since the player has attempt to walk into a wall.
    if (playModel->isWall(TileCoord(newTile->getX(), newTile->getY()))) {
        return true;
    }

    // Move the player to the tile.
    playModel->setPlayerCoord(TileCoord(newTile->getX(), newTile->getY()));

    // Give the player the key if the tile has the key.
    if (playModel->tileCoordHasKey(TileCoord(newTile->getX(), newTile->getY()))) {
        playModel->setPlayerHasKey(true);
    }

    // Handle Teleporter Tiles.
    if (playModel->isTeleporter(playModel->getPlayerCoord())) {
        TileCoord matching = playModel->getMatchingTeleporterTileCoord(playModel->getPlayerCoord());
        playModel->setPlayerCoord(matching);
        return true;
    }

    // This will interrupt movement if the level is over due to prior movement; otherwise movement will not be
    // interrupted
    return playModel->isComplete();
}
