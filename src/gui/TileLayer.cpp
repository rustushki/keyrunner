#include <iostream>
#include "AnimationFactory.hpp"
#include "AnimationType.hpp"
#include "GridLayer.hpp"
#include "TileLayer.hpp"
#include "../game/KeyRunner.hpp"
#include "../model/PlayModel.hpp"

TileLayer::TileLayer(TileType type, uint16_t x, uint16_t y) {
    setType(type);

    this->type = type;
    this->x = x;
    this->y = y;
}

TileLayer::~TileLayer() {
    delete this->anim;
}

/* ------------------------------------------------------------------------------
 * getRect - Get the region of the screen upon which this Layer will be drawn.
 * For InfoBarLayer, it will be at the bottom, vertically below the GridLayer.
 */
SDL_Rect TileLayer::getRect() const {
    SDL_Rect r;
    r.w = TileLayer::SIZE;
    r.h = TileLayer::SIZE;
    r.x = TileLayer::SIZE * x;
    r.y = TileLayer::SIZE * y;
    return r;
}

Animation* TileLayer::getAnimation() const {
    return anim;
}

AnimationType TileLayer::TileTypeToAnimType(TileType tt) {
    switch (tt) {
        case TILETYPE_EMPTY:
            return ANIMATION_TYPE_EMPTY;

        case TILETYPE_WALL:
            return ANIMATION_TYPE_WALL;

        case TILETYPE_DOOR:
            return ANIMATION_TYPE_DOOR;

        case TILETYPE_TELEPORTER_RED:
            return ANIMATION_TYPE_TELEPORTER_RED;

        case TILETYPE_TELEPORTER_GREEN:
            return ANIMATION_TYPE_TELEPORTER_GREEN;

        case TILETYPE_TELEPORTER_BLUE:
            return ANIMATION_TYPE_TELEPORTER_BLUE;

        case TILETYPE_CONVEY_UP:
            return ANIMATION_TYPE_CONVEY_UP;

        case TILETYPE_CONVEY_DOWN:
            return ANIMATION_TYPE_CONVEY_DOWN;

        case TILETYPE_CONVEY_LEFT:
            return ANIMATION_TYPE_CONVEY_LEFT;

        case TILETYPE_CONVEY_RIGHT:
            return ANIMATION_TYPE_CONVEY_RIGHT;

        default:
            std::cout << "Could not determine AnimationType." << std::endl;
            std::cout << "Invalid TileType." << std::endl;
            KeyRunner::exitGame();
            break;
    }

    // Should not arrive here.
    std::cout << "Logic error detected." << std::endl;
    KeyRunner::exitGame();

    // Should certainly never arrive here.
    return ANIMATION_TYPE_EMPTY;
}

TileType TileLayer::getType() const {
    return this->type;
}

void TileLayer::setType(TileType tt) {
    type = tt;

    AnimationType at = TileLayer::TileTypeToAnimType(type);
    this->anim = AnimationFactory::Build(at);

    if (this->anim->isAnimating()) {
        GridLayer::GetInstance()->pushAnimatedTile(this);
    }
}

uint16_t TileLayer::getX() const {
    return this->x;
}

uint16_t TileLayer::getY() const {
    return this->y;
}

void TileLayer::draw(SDL_Surface* dst) {

    const uint16_t tileSize = 25;

    // Determine the coordinate to draw the tile animation..
    uint16_t xp = this->x*tileSize;
    uint16_t yp = this->y*tileSize;

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
