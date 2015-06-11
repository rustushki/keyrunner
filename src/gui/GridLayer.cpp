#include <iostream>

#include "ConveyorAnimation.hpp"
#include "GridLayer.hpp"
#include "TileLayer.hpp"
#include "../game/KeyRunner.hpp"
#include "../game/TileType.hpp"
#include "../model/PlayModel.hpp"

GridLayer* GridLayer::instance = NULL;

GridLayer* GridLayer::GetInstance() {
    if (instance == NULL) {
        instance = new GridLayer();
    }

    return instance;
}

GridLayer::GridLayer() {
    for (int x = 0; x < PlayModel::GRID_WIDTH; x++) {
        for (int y = 0; y < PlayModel::GRID_HEIGHT; y++) {
            tile[y][x] = new TileLayer(TileType::TILETYPE_EMPTY, x, y);
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

/* ------------------------------------------------------------------------------
 * AdvanceAnimatables - Iterate the list of Animations which require advancing,
 * and advance each one.
 */
void GridLayer::animateTiles() {
    for (uint16_t x = 0; x < animatedTiles.size(); x++) {
        TileLayer* tile = animatedTiles[x];
        if (tile->getAnimation()->advance()) {
            addChangedTile(tile);
        }
    }
}

void GridLayer::draw(SDL_Surface* dst) {

    while (!changedTiles.empty()) {

        // Get pair to update.
        TileLayer* t = changedTiles.back();

        // Redraw the tile referenced by that pair.
        t->draw(dst);

        // Remove that pair from the changed tiles list.
        changedTiles.pop_back();
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

/* ------------------------------------------------------------------------------
 * update - Update any data in the GridLayer which affects the pending display
 * of the GridLayer.
 */
void GridLayer::update() {
    // Start any remaining conveyor tiles.
    ConveyorAnimation::StartConveyors();

    // Advance any animated tiles.
    animateTiles();
}

/* ------------------------------------------------------------------------------
 * ClearChangedTiles - Remove all tiles from the changed tile array.
 */
void GridLayer::clearChangedTiles() {
    changedTiles.clear();
}

void GridLayer::addChangedTile(TileLayer* tile) {
    changedTiles.push_back(tile);
}

GridLayer::~GridLayer() {
    clearChangedTiles();
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
        addChangedTile(tile);
    }
}

void GridLayer::refreshTiles() {
    // Blit all Tiles.
    for (int x = 0; x < PlayModel::GRID_WIDTH; x++) {
        for (int y = 0; y < PlayModel::GRID_HEIGHT; y++) {
            addChangedTile(getTile(x, y));
        }
    }
}

bool GridLayer::hasPlayer(int x, int y) const {
    return (tileHasPlayer == getTile(x, y));
}

bool GridLayer::hasKey(int x, int y) const {
    return (tileHasKey == getTile(x, y));
}

/* ------------------------------------------------------------------------------
 * movePlayer - Move the player in the provided direction by one tile.  Handle
 * walls, teleporters, keys and other gameplay elements.
 */
bool GridLayer::movePlayer(Direction d) {

    if (d > DIRECTION_COUNT) {
        std::cout << "Invalid direction." << std::endl;
        KeyRunner::exitGame();
    }

    TileLayer* newTile = NULL;
    if (d == DIRECTION_UP) {
        newTile = tileHasPlayer->up();
    }

    if (d == DIRECTION_DOWN) {
        newTile = tileHasPlayer->down();
    }

    if (d == DIRECTION_LEFT) {
        newTile = tileHasPlayer->left();
    }

    if (d == DIRECTION_RIGHT) {
        newTile = tileHasPlayer->right();
    }

    bool interruptMovement = movePlayerToTile(newTile);

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
    addChangedTile(tileHasPlayer);
    tileHasPlayer = newTile;
    addChangedTile(tileHasPlayer);

    // Give the player the key if the tile has the key.
    if (hasKey(tileHasPlayer->getX(), tileHasPlayer->getY())) {
        tileHasKey = NULL;
        playModel->setPlayerHasKey(true);
    }

    // Handle Teleporter Tiles.
    if (playModel->isTeleporter(TileCoord(tileHasPlayer->getX(), tileHasPlayer->getY()))) {
        TileLayer* matching = getMatchingTeleporterTile(tileHasPlayer);
        tileHasPlayer = matching;
        addChangedTile(tileHasPlayer);
        return true;
    }

    // Interrupt movement if the level is over due to prior movement.
    if (playModel->isComplete()) {
        return true;
    }

    // Normal case, movement is not interrupted.
    return false;
}

/* ------------------------------------------------------------------------------
 * getPlayerTile - Return the current tile of the player.
 */
TileLayer* GridLayer::getPlayerTile() const {
    return tileHasPlayer;
}

/* ------------------------------------------------------------------------------
 * getMatchingTeleporterTile - Given a teleporter tile X and Y, return the
 * matching teleporter tile's X and Y.  Return as a vector int.
 */
TileLayer* GridLayer::getMatchingTeleporterTile(TileLayer* t) {

    TileLayer* matching = NULL;

    // Handle case where a non-telepoprter tile is passed in.  Return the same
    // tile provided.  This should never happen.
    if (!PlayModel::GetInstance()->isTeleporter(TileCoord(t->getX(), t->getY()))) {
        matching = t;


        // Normal case. Find the first matching teleporter tile.
    } else {

        // Search for the matching tile.
        bool found = false;
        for (Uint16 x = 0; x < PlayModel::GRID_WIDTH; x++) {
            for (Uint16 y = 0; y < PlayModel::GRID_HEIGHT; y++) {

                if (x != t->getX() || y != t->getY()) {

                    // Found the a Teleporter TileLayer of the same color which is not this tile.
                    if (t->getType() == getTile(x,y)->getType()) {
                        matching = getTile(x, y);
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
