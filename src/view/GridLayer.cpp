#include <iostream>

#include "../view/GridLayer.hpp"

GridLayer* GridLayer::instance = NULL;

GridLayer* GridLayer::GetInstance() {
    if (instance == NULL) {
        instance = new GridLayer();
    }

    return instance;
}

GridLayer::GridLayer() {
    // GRID_WIDTH and GRID_HEIGHT have been hardcoded to 25 and 16 until GridLayer can be factored out
    for (uint16_t x = 0; x < 25; x++) {
        for (uint16_t y = 0; y < 16; y++) {
            tile[y][x] = new TileLayer(TILE_TYPE_EMPTY, x, y);
        }
    }
}

/* ------------------------------------------------------------------------------
 * PushAnimatedTile - Push the provided animated tile onto the vector of tiles
 * which have animations which require advancing.
 */
void GridLayer::pushAnimatedTile(TileLayer* tile) {
    animatedTiles.push_back(tile);
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
    // GRID_WIDTH and GRID_HEIGHT have been hardcoded to 25 and 16 until GridLayer can be factored out
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 25; x++) {
            tile[y][x]->draw(renderer);
        }
    }
}

SDL_Rect GridLayer::getRect() const {
    // GRID_WIDTH and GRID_HEIGHT have been hardcoded to 25 and 16 until GridLayer can be factored out
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = 25 * TileLayer::SIZE;
    r.h = 16 * TileLayer::SIZE;
    return r;
}

/**
 * Update any data in the GridLayer which affects the pending display of the GridLayer.
 */
void GridLayer::update() {
    // Advance any animated tiles.
    animateTiles();
}

GridLayer::~GridLayer() {
    // GRID_WIDTH and GRID_HEIGHT have been hardcoded to 25 and 16 until GridLayer can be factored out
    for (int x = 0; x < 25; x++) {
        for (int y = 0; y < 16; y++) {
            delete tile[y][x];
        }
    }
}
