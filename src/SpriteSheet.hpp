#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP
#include <string>
#include <SDL/SDL.h>

class SpriteSheet {

public:
	SpriteSheet(std::string filename, uint16_t width, uint16_t height);
	void blitFrame(uint16_t x, uint16_t y, SDL_Rect& where) const; 

	uint16_t getWidth();
	uint16_t getHeight();

private:
	SDL_Surface* sheet;
	uint16_t height;
	uint16_t width;

};

#endif//SPRITESHEET_HPP
