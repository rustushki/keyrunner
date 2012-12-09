#include <SDL/SDL_image.h>
#include "KeyRunner.h"
#include "SpriteSheet.h"

/* ------------------------------------------------------------------------------
 * SpriteSheet - Given a filename, a frame height and frame width, load a
 * sprite sheet into memory with the assumed given frame size.
 */
SpriteSheet::SpriteSheet(std::string filename, uint width, uint height) {
	this->sheet  = IMG_Load(filename.c_str());

	this->width  = width;
	this->height = height;
}

/* ------------------------------------------------------------------------------
 * blitFrame - Find the frame within the sprite sheet denoted by x, y and blit
 * it to the provided SDL_Rect on the screen.  Blocks until the screen is
 * unlocked.
 */
void SpriteSheet::blitFrame(uint x, uint y, SDL_Rect& where) const {

	// Lock the Screen.
	SDL_mutexP(screenLock);

	// Source Rect
	SDL_Rect srcRect;
	srcRect.w = this->width;
	srcRect.h = this->height;
	srcRect.x = x * this->width;
	srcRect.y = y * this->height;

	// Blit the given frame of the sheet to the screen.
	SDL_BlitSurface(this->sheet, &srcRect, screen, &where);

	// Unlock the screen.
	SDL_mutexV(screenLock);

	
}
