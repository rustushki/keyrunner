#ifndef INFOBAR_H
#define INFOBAR_H
#include <SDL/SDL_ttf.h>
#include "KeyRunner.h"
#include "Level.h"

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

	void draw(uint16_t level) const;
	int getHeight() const;

private:
	InfoBar();

	TTF_Font* getFont() const;
	void drawText(std::string s, Position position) const;
	void drawLevel(uint16_t level) const;
	void drawTimer() const;

	static InfoBar* instance;

};

#endif//INFOBAR_H
