#include "InfoBarLayer.hpp"
#include "GridLayer.hpp"
#include "RootLayer.hpp"

InfoBarLayer::InfoBarLayer() {
}

/* ------------------------------------------------------------------------------
 * draw - Draws the InfoBarLayer to the screen.  This includes the level and a
 * timeclock.
 */
void InfoBarLayer::draw(SDL_Renderer* renderer, SDL_Texture* destination) {
    // Build the black bar at the bottom.
    SDL_Rect r = getRect();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderDrawRect(renderer, &r);

    Layer::draw(renderer, destination);

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
