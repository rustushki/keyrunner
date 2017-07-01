#include <sstream>
#include "../controller/KeyRunner.hpp"
#include "../view/PlayInfoBarView.hpp"

/**
 * Constructor.
 * @param model
 * @param rect
 */
PlayInfoBarView::PlayInfoBarView(PlayModel *model, SDL_Rect rect) : BaseView(model, rect) {

}

/**
 * Destructor.
 */
PlayInfoBarView::~PlayInfoBarView() {

}

/**
 * Draw a black bar, the level number and the timer to the bottom of the screen.
 * @param renderer
 */
void PlayInfoBarView::draw(SDL_Renderer *renderer) {
    this->drawBlackBar(renderer);
    this->drawLevel(renderer, getModel()->getBoard()->getLevelNum());
    this->drawTimer(renderer);
}

/**
 * Draws a black bar to the bottom of the screen.
 * @param renderer
 */
void PlayInfoBarView::drawBlackBar(SDL_Renderer* renderer) const {
    // Build the black bar at the bottom.
    SDL_Rect r = getRect();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderDrawRect(renderer, &r);
}

/**
 * Draws the level at the bottom left of the screen.
 * @param renderer
 * @param level the current level
 */
void PlayInfoBarView::drawLevel(SDL_Renderer* renderer, uint16_t level) const {
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
 * Draws a string to a given position. The positions where it may be drawn are simplified to the those described in the
 * InfoBarPos enum. It uses SDL_ttf to create a surface containing text provided by the given string. It then draws this
 * surface to the screen at the given position. The text color is gray.
 * @param renderer surface on which to draw
 * @param text string to draw
 * @param position location on information bar to draw
 */
void PlayInfoBarView::drawText(SDL_Renderer* renderer, std::string text, InfoBarPos position) const {
    // Gray
    SDL_Color color = {0xAA, 0xAA, 0xAA};

    // Build the text surface containing the given string
    SDL_Surface* textSurface = TTF_RenderText_Solid(this->getFont(), text.c_str(), color);

    // If the surface is not created successfully
    if (textSurface == nullptr) {
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
void PlayInfoBarView::drawTimer(SDL_Renderer* renderer) const {
    long currentTimeClock = getModel()->getTimeClock();

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
    int decimal = (int) (currentTimeClock % 1000) / 100;
    timer += (char)decimal + 0x30;

    // Format the Timer String for Display
    timer = "Time: " + timer + " s";

    // Draw it to th screen.
    drawText(renderer, timer, BOTTOM_RIGHT);
}

/**
 * Load a font for usage. Once the font is loaded, keep it statically within to eliminate a global and also prevent
 * reloading the same font.
 * @return TTF_Font*
 */
TTF_Font* PlayInfoBarView::getFont() const {
    // Store loaded font here.
    static TTF_Font* font = nullptr;

    // If the font hasn't been loaded, load it.
    if (font == nullptr) {
        // Is there a way to find these fonts in the filesystem?
        font = TTF_OpenFont(FONT_PATH, 52);
    }

    // Return the font.
    return font;
}

/**
 * Fetch the sub-classed model for this View.
 * @return the model
 */
PlayModel* PlayInfoBarView::getModel() const {
    return (PlayModel*) BaseView::getModel();
}
