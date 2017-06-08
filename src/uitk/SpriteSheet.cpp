#include <iostream>
#include <SDL2/SDL_image.h>

#include "SpriteSheet.hpp"

/**
 * Constructor. Given a filename, a frame height and frame width, load a sprite sheet into memory with the assumed given
 * frame size.
 */
SpriteSheet::SpriteSheet(std::string filename, uint16_t width, uint16_t height) {
    // TODO: Shouldn't there be an SDL_FreeSurface() for this?
    this->sheet = IMG_Load(filename.c_str());
    this->width  = width;
    this->height = height;
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
 * Find the frame within the sprite sheet denoted by x, y and blit it to the provided SDL_Rect on the screen.  Blocks
 * until the screen is unlocked.
 */
void SpriteSheet::blitFrame(SDL_Renderer* renderer, uint16_t frameX, uint16_t frameY, SDL_Rect& where) const {

    // Source Rect
    SDL_Rect srcRect;
    srcRect.w = width;
    srcRect.h = height;
    srcRect.x = frameX * width;
    srcRect.y = frameY * height;

    // Blit the given frame of the sheet to the screen.
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, sheet);
    SDL_RenderCopy(renderer, texture, &srcRect, &where);
}
