#include "GridLayer.hpp"
#include "TileLayer.hpp"
#include "ConveyorAnimation.hpp"

GridLayer* GridLayer::instance = NULL;

GridLayer* GridLayer::GetInstance() {
    if (instance == NULL) {
        instance = new GridLayer();
    }

    return instance;
}

GridLayer::GridLayer() {
    // Do nothing.
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
    r.w = GRID_WIDTH * TileLayer::SIZE;
    r.h = GRID_HEIGHT * TileLayer::SIZE;
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

