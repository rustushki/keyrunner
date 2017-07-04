#include "RectangleView.hpp"

/**
 * Constructor.
 * @param rect
 */
RectangleView::RectangleView(const SDL_Rect &rect) : BaseView(nullptr, rect) {}

/**
 * Destructor.
 */
RectangleView::~RectangleView() {}

/**
 * Draws a rectangle to the screen using the color field.
 * @param renderer
 */
void RectangleView::draw(SDL_Renderer *renderer) {
    uint8_t red = (uint8_t) ((color & 0xFF0000) >> 16);
    uint8_t green = (uint8_t) ((color & 0x00FF00) >> 8);
    uint8_t blue = (uint8_t) ((color & 0x0000FF) >> 0);
    SDL_SetRenderDrawColor(renderer, red, green, blue, 0xFF);
    SDL_Rect rect = getRect();
    SDL_RenderFillRect(renderer, &rect);
}

/**
 * Sets the rectangle color.
 * @param newColor
 */
void RectangleView::setColor(uint32_t newColor) {
    this->color = newColor;
}

