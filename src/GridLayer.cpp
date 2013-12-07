#include "GridLayer.hpp"
#include "Tile.hpp"

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
void GridLayer::pushAnimatedTile(Tile* tile) {
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
        Tile* tile = animatedTiles[x];
        if (tile->getAnimation()->advance()) {
            addChangedTile(tile);
        }
    }
}

void GridLayer::draw(SDL_Surface* dst) {

    while (!changedTiles.empty()) {

        // Get pair to update.
        Tile* t = changedTiles.back();

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
    r.w = GRID_WIDTH * 25;
    r.h = GRID_HEIGHT * 25;
    return r;
}

/* ------------------------------------------------------------------------------
 * ClearChangedTiles - Remove all tiles from the changed tile array.
 */
void GridLayer::clearChangedTiles() {
    changedTiles.clear();
}

void GridLayer::addChangedTile(Tile* tile) {
    changedTiles.push_back(tile);
}

