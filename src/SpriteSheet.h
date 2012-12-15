#ifndef SPRITESHEET_H
#define SPRITESHEET_H
#include <string>
#include <SDL/SDL.h>

typedef unsigned int uint;

class SpriteSheet {

public:
	SpriteSheet(std::string filename, uint width, uint height);
	void blitFrame(uint x, uint y, SDL_Rect& where) const; 

	uint getWidth();
	uint getHeight();

private:
	SDL_Surface* sheet;
	uint height;
	uint width;

};

#endif//SPRITESHEET_H
