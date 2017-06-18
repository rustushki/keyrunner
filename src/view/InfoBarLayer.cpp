#include "InfoBarLayer.hpp"
#include "GridLayer.hpp"

InfoBarLayer::InfoBarLayer() {
}

/**
 * Draws a black bar on the bottom of the screen.
 * @param renderer
 */
void InfoBarLayer::draw(SDL_Renderer* renderer) {
    // Build the black bar at the bottom.
    SDL_Rect r = getRect();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderDrawRect(renderer, &r);

    Layer::draw(renderer);

}

/**
 * Get the region of the screen upon which this Layer will be drawn. For InfoBarLayer, it will be at the bottom,
 * vertically below the GridLayer.
 */
SDL_Rect InfoBarLayer::getRect() const {
    SDL_Rect r;
    r.x = 0;
    r.y = GridLayer::GetInstance()->getRect().h;
    r.h = 40;
    r.w = GridLayer::GetInstance()->getRect().w;
    return r;
}
