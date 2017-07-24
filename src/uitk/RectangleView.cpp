#include "RectangleView.hpp"

/**
 * Constructor.
 * @param rect
 */
RectangleView::RectangleView(Model* model, const SDL_Rect &rect) : BaseView(model, rect) {
    // Ensure default background color is initialized to black
    setColor(0);
}

/**
 * Draws a rectangle to the screen using the color field.
 * @param renderer
 */
void RectangleView::draw(SDL_Renderer *renderer) {
    uint8_t red = (uint8_t) ((color & 0xFF0000) >> 16);
    uint8_t green = (uint8_t) ((color & 0x00FF00) >> 8);
    uint8_t blue = (uint8_t) ((color & 0x0000FF) >> 0);
    SDL_SetRenderDrawColor(renderer, red, green, blue, 0xFF);
    SDL_Rect rect = {getX(), getY(), getWidth(), getHeight()};
    SDL_RenderFillRect(renderer, &rect);
}

/**
 * Sets the rectangle color.
 * @param newColor
 */
void RectangleView::setColor(uint32_t newColor) {
    this->color = newColor;
}

/**
 * Get the rectangle's color.
 * @return
 */
uint32_t RectangleView::getColor() const {
    return this->color;
}

