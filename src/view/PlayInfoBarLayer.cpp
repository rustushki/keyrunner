#include <sstream>
#include "PlayInfoBarLayer.hpp"
#include "RootLayer.hpp"

PlayInfoBarLayer* PlayInfoBarLayer::instance = 0;

/**
 * Singleton getter.
 * @return the singleton instance of this information bar
 */
PlayInfoBarLayer* PlayInfoBarLayer::GetInstance() {
    if (PlayInfoBarLayer::instance == 0) {
        PlayInfoBarLayer::instance = new PlayInfoBarLayer();
    }

    return PlayInfoBarLayer::instance;
}

/**
 * Constructor.
 */
PlayInfoBarLayer::PlayInfoBarLayer() {}

/**
 * Destructor.
 */
PlayInfoBarLayer::~PlayInfoBarLayer() {}

/**
 * Draws the timer and the level onto the Play Information Bar.
 * @param dst surface on which to draw
 */
void PlayInfoBarLayer::draw(SDL_Renderer* renderer) {
    InfoBarLayer::draw(renderer);

    // As they say.
    this->drawLevel(renderer, PlayModel::GetInstance()->getLevelNum());
    this->drawTimer(renderer);

}

/**
 * Draws the level at the bottom left of the screen.
 * @param dst the surface on which to draw
 * @param level the current level
 */
void PlayInfoBarLayer::drawLevel(SDL_Renderer* renderer, uint16_t level) const {

    // Covert the level into a string.
    std::string levelStr = "";
    while (level >= 1) {
        levelStr.insert(levelStr.begin(), (char)((level % 10) + 0x30));
        level /= 10;
    }

    // Prepend "Level: " onto the string.
    levelStr = "Level: " + levelStr;

    // Draw the text to the screen.
    drawText(renderer, levelStr, BOTTOM_LEFT);
}

/**
 * Draws a string to a given position.  The positions where it may be drawn are simplified to the those described in the
 * InfoBarPos enum.  See it for details.  Uses SDL_ttf to create a surface containing text provided by the given string.
 * It then blits this surface to the screen at the given position.  Color is assumed gray for now.
 * @param destination surface on which to draw
 * @param s string to draw
 * @param position location on information bar to draw
 */
void PlayInfoBarLayer::drawText(SDL_Renderer* renderer, std::string s, InfoBarPos position) const {
    // Gray
    SDL_Color color = {0xAA, 0xAA, 0xAA};

    // Build the text surface containing the given string
    SDL_Surface* textSurface = TTF_RenderText_Solid(this->getFont(), s.c_str(), color);

    // If the surface is not created successfully
    if (textSurface == NULL) {
        std::stringstream errorMessage;
        errorMessage << "Error creating text: " << TTF_GetError();
        throw std::runtime_error(errorMessage.str());

    // Otherwise,
    } else {
        const uint16_t marginTop = 5;

        // Create a destination rectangle based on the created text surface and the position parameter
        SDL_Rect r;
        r.w = (uint16_t) textSurface->w;
        r.h = (uint16_t) textSurface->h;
        r.y = getRect().y + marginTop;

        // Determine the x and y coordinates of the text based on the provided position
        SDL_Rect rect = getRect();
        if (position == BOTTOM_LEFT) {
            r.x = rect.x;
        } else if (position == BOTTOM_RIGHT) {
            r.x = rect.w - textSurface->w;
        } else if (position == BOTTOM_CENTER) {
            r.x = (rect.w - textSurface->w) / 2;
        } else if (position == MIDDLE_CENTER) {
            r.x = (rect.w - textSurface->w) / 2;
            r.y = (rect.h - textSurface->h) / 2;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        // Draw the text to the screen
        SDL_RenderCopy(renderer, textTexture, nullptr, &r);

        // Free the text surface and texture
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

/**
 * Draws the timer at the bottom right of the screen.
 * @param destination surface on which to draw
 */
void PlayInfoBarLayer::drawTimer(SDL_Renderer* renderer) const {
    uint32_t currentTimeClock = PlayModel::GetInstance()->getTimeClock();

    // Convert the timeout into a string.
    std::string timer = "";
    float time = (float) currentTimeClock / 1000;

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
    int decimal = (currentTimeClock % 1000) / 100;
    timer += (char)decimal + 0x30;

    // Format the Timer String for Display
    timer = "Time: " + timer + " s";

    // Draw it to th screen.
    drawText(renderer, timer, BOTTOM_RIGHT);
}

/**
 * Load a font for usage.  Once the font is loaded, keep it statically within to eliminate a global and also prevent
 * reloading the same font.
 * @return TTF_Font*
 */
TTF_Font* PlayInfoBarLayer::getFont() const {
    // Store loaded font here.
    static TTF_Font* font = NULL;

    // If the font hasn't been loaded, load it.
    if (font == NULL) {

        // Is there a way to find these fonts in the filesystem?
        font = TTF_OpenFont(FONT_PATH, 52);

    }

    // Return the font.
    return font;
}
