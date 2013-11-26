#ifndef INFOBAR_HPP
#define INFOBAR_HPP
#include <SDL/SDL_ttf.h>
#include "KeyRunner.hpp"
#include "Level.hpp"

// Position indicates where text should be drawn on the screen.
enum Position {
	  BOTTOM_LEFT
	, BOTTOM_RIGHT
	, BOTTOM_CENTER
	, MIDDLE_CENTER
};

class InfoBar {

public:
	static InfoBar* GetInstance();
	~InfoBar();

	SDL_Surface* getSurface(uint16_t level) const;
	int getHeight() const;
	int getWidth() const;
	int getX() const;
	int getY() const;

private:
	InfoBar();

	TTF_Font* getFont() const;
	void drawText(std::string s, Position position) const;
	void drawLevel(uint16_t level) const;
	void drawTimer() const;

	static InfoBar* instance;

	SDL_Surface* ibSrf;

};

#endif//INFOBAR_HPP
