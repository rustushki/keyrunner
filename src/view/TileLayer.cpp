#include <iostream>
#include "AnimationFactory.hpp"
#include "GridLayer.hpp"

/**
 * Constructor.
 * @param tileType
 * @param x
 * @param y
 */
TileLayer::TileLayer(TileType tileType, uint16_t x, uint16_t y) {
    setType(tileType);

    this->tileType = tileType;
    this->x = x;
    this->y = y;
}

/**
 * Destructor.
 */
TileLayer::~TileLayer() {
    delete this->animation;
}

/**
 * Get the region of the screen upon which this Layer will be drawn. For InfoBarLayer, it will be at the bottom,
 * vertically below the GridLayer.
 */
SDL_Rect TileLayer::getRect() const {
    SDL_Rect r;
    r.w = SIZE;
    r.h = SIZE;
    r.x = SIZE * x;
    r.y = SIZE * y;
    return r;
}

/**
 * Get the animation to be used with this TileLayer.
 * @return Animation
 */
Animation* TileLayer::getAnimation() const {
    return animation;
}

/**
 * Get the TileType of this TileLayer.
 * @return TileType
 */
TileType TileLayer::getType() const {
    return this->tileType;
}

/**
 * Set the TileType of this TileLayer (changing the Animation)
 * @param tileType the new TileType
 */
void TileLayer::setType(TileType tileType) {
    this->tileType = tileType;

    AnimationType at = tileType.toAnimationType();
    this->animation = AnimationFactory::Build(at);

    if (this->animation->isAnimating()) {
        GridLayer::GetInstance()->pushAnimatedTile(this);
    }
}

/**
 * Get the x coordinate of this TileLayer
 * @return uint16_t
 */
uint16_t TileLayer::getX() const {
    return this->x;
}

/**
 * Get the y coordinate of this TileLayer
 * @return uint16_t
 */
uint16_t TileLayer::getY() const {
    return this->y;
}

/**
 * Draw the TileLayer onto its destination surface.
 * @param dst the destination surface
 */
void TileLayer::draw(SDL_Surface* dst) {
    const uint16_t tileSize = 25;

    // Determine the coordinate to draw the tile animation..
    uint16_t xp = this->x * tileSize;
    uint16_t yp = this->y * tileSize;

    this->getAnimation()->move(xp, yp);
    this->getAnimation()->blit(dst);

    PlayModel* playModel = PlayModel::GetInstance();

    // Redraw the Key.
    if (playModel->tileCoordHasKey(TileCoord(this->x, this->y))) {
        KeyAnim->move(xp, yp);
        KeyAnim->blit(dst);
    }

    // Redraw the Player.
    if (playModel->tileCoordHasPlayer(TileCoord(this->x, this->y))) {
        PlayerAnim->move(xp, yp);
        PlayerAnim->blit(dst);
    }
}
