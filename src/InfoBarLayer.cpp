#include "InfoBarLayer.hpp"

InfoBarLayer* InfoBarLayer::instance = 0;

InfoBarLayer* InfoBarLayer::GetInstance() {
    if (InfoBarLayer::instance == 0) {
        InfoBarLayer::instance = new InfoBarLayer();
    }

    return InfoBarLayer::instance;
}

InfoBarLayer::InfoBarLayer() {
}

InfoBarLayer::~InfoBarLayer() {
}

/* ------------------------------------------------------------------------------
 * draw - Draws the InfoBarLayer to the screen.  This includes the level and a
 * timeclock.
 */
void InfoBarLayer::draw(SDL_Surface* dst) {

    // Build the black bar at the bottom.
    SDL_Rect r;
    r.x = getX();
    r.y = getY();
    r.w = getWidth();
    r.h = getHeight();
    SDL_FillRect(dst, &r, 0x000000);

    // As they say.
    this->drawLevel(dst, KeyRunner::getLevelNum());
    this->drawTimer(dst);

}

/* ------------------------------------------------------------------------------
 * getWidth() - Return the width of the InfoBarLayer.
 */
int InfoBarLayer::getWidth() const {
    return KeyRunner::getWidth();
}

/* ------------------------------------------------------------------------------
 * getHeight() - Return the height of the InfoBarLayer.
 */
int InfoBarLayer::getHeight() const {
    return 40;
}

/* ------------------------------------------------------------------------------
 * getX() - Return the X of the info bar relative to the whole screen.
 */
int InfoBarLayer::getX() const {
    return 0;
}

/* ------------------------------------------------------------------------------
 * getY() - Return the Y of the info bar relative to the whole screen.
 */
int InfoBarLayer::getY() const {
    return KeyRunner::getHeight() - getHeight();
}

/* ------------------------------------------------------------------------------
 * drawLevel - draws the level at the bottom left of the screen.
 */
void InfoBarLayer::drawLevel(SDL_Surface* dst, uint16_t level) const {

    // Covert the level into a string.
    std::string levelStr = "";
    while (level >= 1) {
        levelStr.insert(levelStr.begin(), (char)((level % 10) + 0x30));
        level /= 10;
    }

    // Prepend "Level: " onto the string.
    levelStr = "Level: " + levelStr;

    // Draw the text to the screen.
    drawText(dst, levelStr, BOTTOM_LEFT);
}

/* ------------------------------------------------------------------------------
 * drawText - Draws a string to a given position.  The positions where it may
 * be drawn are simplified to the those described in the InfoBarPos enum.  See
 * it for details.  Uses SDL_ttf to create a surface containing text provided
 * by the given string.  It then blits this surface to the screen at the given
 * position.  Color is assumed gray for now.
 */
void InfoBarLayer::drawText(SDL_Surface* dst, std::string s, InfoBarPos position) const {

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
        r.y = getY() + marginTop;

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
        SDL_BlitSurface(text_surface, NULL, dst, &r);
    }

}

/* ------------------------------------------------------------------------------
 * drawTimer - draws the timer at the bottom right of the screen.
 */
void InfoBarLayer::drawTimer(SDL_Surface* dst) const {

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
    drawText(dst, timer, BOTTOM_RIGHT);
}

/* ------------------------------------------------------------------------------
 * getFont - Load a font for usage.  Once the font is loaded, keep it
 * statically within to eliminate a global and also prevent re-loading the same
 * font.
 */
TTF_Font* InfoBarLayer::getFont() const {
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
