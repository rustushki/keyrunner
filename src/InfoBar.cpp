#include "InfoBar.hpp"

InfoBar* InfoBar::instance = 0;

InfoBar* InfoBar::GetInstance() {
    if (InfoBar::instance == 0) {
        InfoBar::instance = new InfoBar();
    }

    return InfoBar::instance;
}

InfoBar::InfoBar() {
    ibSrf = SDL_CreateRGBSurface(0, getWidth(), getHeight()
            , 32, 0, 0, 0, 0);
}

InfoBar::~InfoBar() {
    SDL_FreeSurface(ibSrf);
}

/* ------------------------------------------------------------------------------
 * getSurface - Returns a surface containing the graphical representation of
 * the InfoBar.  Returned memory should not be deallocated by caller.
 */
SDL_Surface* InfoBar::getSurface(uint16_t level) const {

    // Build the black bar at the bottom.
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = getWidth();
    r.h = getHeight();
    SDL_FillRect(ibSrf, &r, 0x000000);

    // As they say.
    this->drawLevel(level);
    this->drawTimer();

    return ibSrf;
}

/* ------------------------------------------------------------------------------
 * getWidth() - Return the width of the InfoBar.
 */
int InfoBar::getWidth() const {
    return KeyRunner::getWidth();
}

/* ------------------------------------------------------------------------------
 * getHeight() - Return the height of the InfoBar.
 */
int InfoBar::getHeight() const {
    return 40;
}

/* ------------------------------------------------------------------------------
 * getX() - Return the X of the info bar relative to the whole screen.
 */
int InfoBar::getX() const {
    return 0;
}

/* ------------------------------------------------------------------------------
 * getY() - Return the Y of the info bar relative to the whole screen.
 */
int InfoBar::getY() const {
    return KeyRunner::getHeight() - getHeight();
}

/* ------------------------------------------------------------------------------
 * drawLevel - draws the level at the bottom left of the screen.
 */
void InfoBar::drawLevel(uint16_t level) const {

    // Covert the level into a string.
    std::string levelStr = "";
    while (level >= 1) {
        levelStr.insert(levelStr.begin(), (char)((level % 10) + 0x30));
        level /= 10;
    }

    // Prepend "Level: " onto the string.
    levelStr = "Level: " + levelStr;

    // Draw the text to the screen.
    drawText(levelStr, BOTTOM_LEFT);
}

/* ------------------------------------------------------------------------------
 * drawText - Draws a string to a given position.  The positions where it may
 * be drawn are simplified to the those described in the InfoBarPos enum.  See
 * it for details.  Uses SDL_ttf to create a surface containing text provided
 * by the given string.  It then blits this surface to the screen at the given
 * position.  Color is assumed gray for now.
 */
void InfoBar::drawText(std::string s, InfoBarPos position) const {

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
        const uint16_t marginTop = 5;

        // Create a destination rectangle based on the created text surface and
        // the position parameter.
        SDL_Rect r;
        r.w = text_surface->w;
        r.h = text_surface->h;
        r.y = marginTop;

        if (position == BOTTOM_LEFT) {
            r.x = 0;
        } else if (position == BOTTOM_RIGHT) {
            r.x = KeyRunner::getWidth() - text_surface->w;
        } else if (position == BOTTOM_CENTER) {
            r.x = (KeyRunner::getWidth() - text_surface->w)/2;
        } else if (position == MIDDLE_CENTER) {
            r.x = (KeyRunner::getWidth() - text_surface->w)/2;
            r.y = (KeyRunner::getHeight() - text_surface->h)/2;
        }

        // Blit the text to the screen.
        SDL_BlitSurface(text_surface, NULL, ibSrf, &r);
    }

}

/* ------------------------------------------------------------------------------
 * drawTimer - draws the timer at the bottom right of the screen.
 */
void InfoBar::drawTimer() const {

    // Convert the timeout into a string.
    std::string timer = "";
    float time = (float)KeyRunner::getTimeClock()/1000;

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
    int decimal = (KeyRunner::getTimeClock() % 1000) / 100;
    timer += (char)decimal + 0x30;

    // Format the Timer String for Display
    timer = "Time: " + timer + " s";

    // Draw it to th screen.
    drawText(timer, BOTTOM_RIGHT);
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
