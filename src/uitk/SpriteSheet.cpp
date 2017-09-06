#include <SDL2/SDL_image.h>

#include "../uitk/SpriteSheet.hpp"

/**
 * Constructor. Given a filename, a frame height and frame width, load a sprite sheet into memory with the assumed given
 * frame size.
 * @param filename path to sprite sheet
 * @param width width of sprite sheet
 * @param height height of sprite sheet
 */
SpriteSheet::SpriteSheet(SDL_Renderer* renderer, std::string filename, uint16_t width, uint16_t height) {
    SDL_Surface* surface = IMG_Load(filename.c_str());
    this->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    this->width  = width;
    this->height = height;
}

/**
 * Destructor.
 * <p>
 * Ensure the local texture is freed.
 */
SpriteSheet::~SpriteSheet() {
    SDL_DestroyTexture(this->texture);
}

/**
 * Return the width of the frame size.
 */
uint16_t SpriteSheet::getWidth() const {
    return width;
}

/**
 * Return the height of the frame size.
 */
uint16_t SpriteSheet::getHeight() const {
    return height;
}

/**
 * Find the still within the sprite sheet denoted by x, y and draw it to the provided SDL_Rect on the screen.
 *
 * @param renderer used to draw to the back buffer
 * @param stillX X coordinate of still to draw
 * @param stillY Y coordinate of still to draw
 * @param where draw still to this destination rectangle
 */
void SpriteSheet::drawStill(SDL_Renderer *renderer, uint16_t stillX, uint16_t stillY, SDL_Rect &where) {
    // Source Rect
    SDL_Rect source = {stillX * width, stillY * width, width, height};

    // Draw the given still of the sprite sheet to the back buffer
    SDL_RenderCopy(renderer, texture, &source, &where);
}
