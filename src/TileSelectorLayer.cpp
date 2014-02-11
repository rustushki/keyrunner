#include "TileSelectorLayer.hpp"

TileSelectorLayer::TileSelectorLayer() {
}

/* ------------------------------------------------------------------------------
 * draw - Draws the TileSelectorLayer to the screen.  This is mainly a black
 * box, with a gray border.  Inside is a horizontal list of Tiles which may be
 * selected.
 */
void TileSelectorLayer::draw(SDL_Surface* dst) {
    // Build the black bar at the bottom.
    SDL_Rect r = getRect();
    SDL_FillRect(dst, &r, 0x00FF00);

    Layer::draw(dst);

}

/* ------------------------------------------------------------------------------
 * getRect - Get the region of the screen upon which this Layer will be drawn.
 * For TileSelectorLayer, it will be at the bottom, taking up some portion of
 * the InfoBarLayer.
 */
SDL_Rect TileSelectorLayer::getRect() const {
    const uint8_t margin = 5;

    SDL_Rect r;
    r.x = margin;
    r.y = GridLayer::GetInstance()->getRect().h + margin;
    r.h = 40 - margin*2;
    r.w = GridLayer::GetInstance()->getRect().w - margin*2;
    return r;
}

