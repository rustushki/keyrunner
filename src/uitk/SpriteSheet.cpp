#include <SDL/SDL_image.h>

#include "SpriteSheet.hpp"

/* ------------------------------------------------------------------------------
 * SpriteSheet - Given a filename, a frame height and frame width, load a
 * sprite sheet into memory with the assumed given frame size.
 */
SpriteSheet::SpriteSheet(std::string filename, uint16_t width, uint16_t height) {
    this->sheet  = IMG_Load(filename.c_str());

    this->width  = width;
    this->height = height;
}

/* ------------------------------------------------------------------------------
 * getWidth - Return the width of the frame size.
 */
uint16_t SpriteSheet::getWidth() const {
    return width;
}

/* ------------------------------------------------------------------------------
 * getHeight - Return the height of the frame size.
 */
uint16_t SpriteSheet::getHeight() const {
    return height;
}

/* ------------------------------------------------------------------------------
 * blitFrame - Find the frame within the sprite sheet denoted by x, y and blit
 * it to the provided SDL_Rect on the screen.  Blocks until the screen is
 * unlocked.
 */
void SpriteSheet::blitFrame(uint16_t frameX, uint16_t frameY, SDL_Surface* dst, SDL_Rect& where) const {

    // Source Rect
    SDL_Rect srcRect;
    srcRect.w = width;
    srcRect.h = height;
    srcRect.x = frameX * width;
    srcRect.y = frameY * height;

    // Blit the given frame of the sheet to the screen.
    SDL_BlitSurface(sheet, &srcRect, dst, &where);

}