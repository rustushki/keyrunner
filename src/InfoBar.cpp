#include "InfoBar.h"

InfoBar* InfoBar::instance = 0;

InfoBar* InfoBar::GetInstance() {
	if (InfoBar::instance == 0) {
		InfoBar::instance = new InfoBar();
	}

	return InfoBar::instance;
}

InfoBar::InfoBar() {

}

/* ------------------------------------------------------------------------------
 * draw - Draws the information bar onto the screen at the bottom.
 */
void InfoBar::draw() const {

	SDL_mutexP(screenLock);

	// Build the black bar at the bottom.
	SDL_Rect r;
	r.x = 0;
	r.y = ::getHeight() - this->getHeight();
	r.w = getWidth();
	r.h = this->getHeight();
	SDL_FillRect(screen, &r, 0x000000);

	// As they say.
	this->drawLevel();
	this->drawTimer();

	SDL_mutexV(screenLock);

}

/* ------------------------------------------------------------------------------
 * drawLevel - draws the level at the bottom left of the screen.
 */
void InfoBar::drawLevel() const {

	// Covert the level into a string.
	int lev = level.toInt();
	std::string levelStr = "";
	while (lev >= 1) {
		levelStr.insert(levelStr.begin(), (char)((lev % 10) + 0x30));
		lev /= 10;
	}

	// Prepend "Level: " onto the string.
	levelStr = "Level: " + levelStr;

	// Draw the text to the screen.
	drawText(levelStr, BOTTOM_LEFT);
}

/* ------------------------------------------------------------------------------
 * drawText - Draws a string to a given position.  The positions where it may
 * be drawn are simplified to the those described in the Position enum.  See it
 * for details.  Uses SDL_ttf to create a surface containing text provided by
 * the given string.  It then blits this surface to the screen at the given
 * position.  Color is assumed gray for now.
 */
void InfoBar::drawText(std::string s, Position position) const {

	// Gray
	SDL_Color color = {0xAA, 0xAA, 0xAA};

	// Build the text surface containing the given string.
	SDL_Surface* text_surface = TTF_RenderText_Solid(this->getFont(), s.c_str(), color);

	// If the surface is not created successfully.
	if (text_surface == NULL) {
		std::cout << s.c_str() << std::endl;
		std::cout << "Error creating text: " << TTF_GetError() << std::endl;
		exit(2);
	
	// Otherwise,
	} else {

		// Create a destination rectangle based on the created text surface and
		// the position parameter.
		SDL_Rect r;
		r.w = text_surface->w;
		r.h = text_surface->h;
		r.y = ::getHeight() - text_surface->h;

		if (position == BOTTOM_LEFT) {
			r.x = 0;
		} else if (position == BOTTOM_RIGHT) {
			r.x = getWidth() - text_surface->w;
		} else if (position == BOTTOM_CENTER) {
			r.x = (getWidth() - text_surface->w)/2;
		} else if (position == MIDDLE_CENTER) {
			r.x = (getWidth() - text_surface->w)/2;
			r.y = (::getHeight() - text_surface->h)/2;
		}

		// Blit the text to the screen.
		SDL_BlitSurface(text_surface, NULL, screen, &r);
	}

}

/* ------------------------------------------------------------------------------
 * drawTimer - draws the timer at the bottom right of the screen.
 */
void InfoBar::drawTimer() const {

	// Convert the timeout into a string.
	std::string timer = "";
	float time = (float)timeClock/1000;

	if (time >= 1) {
		while (time >= 1) {
			timer.insert(timer.begin(), ((char)(((int)time % 10) + 0x30)));
			time /= 10;
		}
	} else {
		timer = "0";
	}

	// Add a decimal.
	timer += ".";

	// Get the tenths place.
	int decimal = (timeClock % 1000) / 100;
	timer += (char)decimal + 0x30;

	// Format the Timer String for Display
	timer = "Time: " + timer + " s";

	// Draw it to th screen.
	drawText(timer, BOTTOM_RIGHT);
}

int InfoBar::getHeight() const {
	return 40;
}

/* ------------------------------------------------------------------------------
 * getFont - Load a font for usage.  Once the font is loaded, keep it
 * statically within to eliminate a global and also prevent re-loading the same
 * font.
 */
TTF_Font* InfoBar::getFont() const {
	// Store loaded font here.
	static TTF_Font* font = NULL;

	// If the font hasn't been loaded, load it.
	if (font == NULL) {

		// Is there a way to find these fonts in the filesystem?
		font = TTF_OpenFont(FONTPATH, 52);

	}

	// Return the font.
	return font;
}
