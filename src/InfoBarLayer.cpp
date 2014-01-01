#include "InfoBarLayer.hpp"
#include "RootLayer.hpp"

InfoBarLayer::InfoBarLayer() {
}

/* ------------------------------------------------------------------------------
 * draw - Draws the InfoBarLayer to the screen.  This includes the level and a
 * timeclock.
 */
void InfoBarLayer::draw(SDL_Surface* dst) {
    // Build the black bar at the bottom.
    SDL_Rect r = getRect();
    SDL_FillRect(dst, &r, 0x000000);

    Layer::draw(dst);

}

/* ------------------------------------------------------------------------------
 * getRect - Get the region of the screen upon which this Layer will be drawn.
 * For InfoBarLayer, it will be at the bottom, vertically below the GridLayer.
 */
SDL_Rect InfoBarLayer::getRect() const {
    SDL_Rect r;
    r.x = 0;
    r.y = GridLayer::GetInstance()->getRect().h;
    r.h = 40;
    r.w = GridLayer::GetInstance()->getRect().w;
    return r;
}
