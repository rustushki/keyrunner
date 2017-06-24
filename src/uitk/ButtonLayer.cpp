#include <math.h>
#include <sstream>

#include "Animation.hpp"
#include "ButtonLayer.hpp"

ButtonLayer::ButtonLayer() {
    horizontalMargin   = 20;
    verticalMargin   = 20;
    textDirty = true;
    textTexture = NULL;
    icon = NULL;
}

ButtonLayer::~ButtonLayer() {
    if (icon != NULL) {
        delete icon;
    }
}

SDL_Rect ButtonLayer::getRect() const {
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = width;
    r.h = height;
    return r;
}

void ButtonLayer::update() {
}

/* ------------------------------------------------------------------------------
 * draw - Given a surface, draw a button onto the surface.
 */
void ButtonLayer::draw(SDL_Renderer* renderer) {
    // Width of the button shading.  Anything other than 1 looks ugly.
    const uint8_t shadeWidth = 1;

    SDL_Rect fillRect = getRect();

    // Draw the Upper Shadow
    if (isSelected()) {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);

    } else {
        SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
    }
    SDL_RenderDrawRect(renderer, &fillRect);

    // Draw the Lower Shadow
    if (isSelected()) {
        SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);

    } else {
        SDL_SetRenderDrawColor(renderer, 0x44, 0x44, 0x44, 0xFF);
    }
    fillRect.x += shadeWidth;
    fillRect.y += shadeWidth;
    SDL_RenderDrawRect(renderer, &fillRect);

    // Draw Background
    uint8_t rC = (uint8_t) ((bgColor & 0xFF0000) >> 16);
    uint8_t gC = (uint8_t) ((bgColor & 0x00FF00) >>  8);
    uint8_t bC = (uint8_t) ((bgColor & 0x0000FF) >>  0);
    SDL_SetRenderDrawColor(renderer, rC, gC, bC, 0xFF);
    fillRect.h -= shadeWidth;
    fillRect.w -= shadeWidth;
    SDL_RenderDrawRect(renderer, &fillRect);

    if (textDirty) {
        // Build the text surface containing the given string.
        textTexture = sizeText(renderer, buttonText);
        textDirty = false;
    }

    // Draw the text centered within the button, obeying the margin.
    // A NULL textTexture implies the text is a 0-length string.
    if (textTexture != nullptr) {
        // Determine width and height of the text
        int w;
        int h;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &w, &h);

        // Determine the rectangle that the text will be drawn onto
        SDL_Rect textDestination = getRect();
        textDestination.x += round((textDestination.w - w) / 2.0) + shadeWidth;
        textDestination.y += round((textDestination.h - h) / 2.0) + shadeWidth;
        textDestination.h = h;
        textDestination.w = w;

        // Draw the text into that rectangle
        SDL_RenderCopy(renderer, textTexture, nullptr, &textDestination);

    // OR,
    // Draw the Icon, centered.
    } else if (icon != NULL) {
        uint16_t x = (uint16_t) (fillRect.x + (fillRect.w - icon->getWidth()) / 2);
        uint16_t y = (uint16_t) (fillRect.y + (fillRect.h - icon->getHeight()) / 2);
        icon->move(x, y);
        icon->draw(renderer);
    }
}

/* ------------------------------------------------------------------------------
 * getFont - Load a font for usage.  Once the font is loaded, keep it
 * statically within to eliminate a global and also prevent re-loading the same
 * font.
 */
TTF_Font* ButtonLayer::getFont(uint8_t size) const {
    return TTF_OpenFont(fontPath.c_str(), size);
}

void ButtonLayer::setBackgroundColor(uint32_t color) {
    bgColor = color;
}

void ButtonLayer::setTextColor(uint32_t color) {
    textDirty = true;
    textColor = color;
}

/* ------------------------------------------------------------------------------
 * setText - Change the text on the button to the value of the provided string.
 */
void ButtonLayer::setText(std::string text) {
    textDirty = true;
    buttonText = text;
}

/* ------------------------------------------------------------------------------
 * setHeight - Change the height of the button to the provided height.
 */
void ButtonLayer::setHeight(uint16_t newHeight) {
    textDirty = true;
    height = newHeight;
}

/* ------------------------------------------------------------------------------
 * setWidth - Change the width of the button to the provided width.
 */
void ButtonLayer::setWidth(uint16_t newWidth) {
    textDirty = true;
    width = newWidth;
}

void ButtonLayer::setX(uint16_t pX) {
    textDirty = true;
    x = pX;
}

void ButtonLayer::setY(uint16_t pY) {
    textDirty = true;
    y = pY;
}

void ButtonLayer::setMarginHorizontal(uint16_t marginHorizontal) {
    horizontalMargin = marginHorizontal;
}

void ButtonLayer::setMarginVertical(uint16_t marginVertical) {
    verticalMargin = marginVertical;
}

/* ------------------------------------------------------------------------------
 * Perform binary searches to build a text surface which will fit into the
 * ButtonLayer horizontally and vertically.
 *
 * Return that text surface.
 *
 * Maintenance note: If implementing a mutator which affects text appearance,
 * be sure to set textDirty = true.
 */
SDL_Texture* ButtonLayer::sizeText(SDL_Renderer* renderer, std::string text) const {
    if (text.empty()) {
        return NULL;
    }

    // Lo, Hi and Mid variables for the binary search.
    uint8_t lo = 1;
    uint8_t hi = 255;
    uint8_t mi = lo;

    // Width and Height vars which will store the calculated width and height
    // of rendered text surfaces.
    int w = 0;
    int h = 1;

    // Determine the region of the button to fill with text based on the margin
    // and button height/width.
    SDL_Rect r = getRect();
    int fillH = r.w - horizontalMargin;
    int fillV = r.h - verticalMargin;

    // Calculate the largest font size which will fit the ButtonLayer surface
    // height-wise.
    while (lo < hi && h != fillV) {
        mi = (uint8_t) ((hi - lo) / 2) + lo;

        TTF_Font* fnt = getFont(mi);
        TTF_SizeText(fnt, text.c_str(), &w, &h);
        TTF_CloseFont(fnt);

        if (h > fillV) {
            hi = (uint8_t) (mi - 1);
        } else if (h < fillV) {
            lo = (uint8_t) (mi + 1);
        }
    }

    // Calculate the largest font size which will fit the ButtonLayer surface
    // width-wise.
    lo = 1;
    hi = mi;
    while (lo < hi && w != fillH) {
        mi = (uint8_t) ((hi - lo) / 2) + lo;

        TTF_Font* fnt = getFont(mi);
        TTF_SizeText(fnt, text.c_str(), &w, &h);
        TTF_CloseFont(fnt);

        if (w > fillH) {
            hi = (uint8_t) (mi - 1);
        } else if (w < fillH) {
            lo = (uint8_t) (mi + 1);
        }
    }

    // Render and return the text surface which fits in the ButtonLayer.
    TTF_Font* fnt = getFont(mi);
    uint8_t rC = (uint8_t) ((textColor & 0xFF0000) >> 16);
    uint8_t gC = (uint8_t) ((textColor & 0x00FF00) >>  8);
    uint8_t bC = (uint8_t) ((textColor & 0x0000FF) >>  0);
    SDL_Color color = {rC, gC, bC};
    SDL_Surface* textSurface = TTF_RenderText_Solid(fnt, text.c_str(), color);
    TTF_CloseFont(fnt);

    // Confirm that the text surface was created.  If not something is horribly
    // wrong, so die.
    if (textSurface == nullptr) {
        std::stringstream message;
        message << "Error drawing text: " << TTF_GetError();
        throw std::runtime_error(message.str());
    }

    return SDL_CreateTextureFromSurface(renderer, textSurface);
}

void ButtonLayer::setIcon(Animation* animation) {
    icon = animation;
}

void ButtonLayer::setFontPath(std::string fontPath) {
    this->fontPath = fontPath;
}
