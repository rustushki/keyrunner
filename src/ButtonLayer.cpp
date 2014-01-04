#include <math.h>

#include "ButtonLayer.hpp"
#include "GridLayer.hpp"

ButtonLayer::ButtonLayer(std::string text, uint32_t bgColor, uint32_t
        textColor, uint16_t width, uint16_t height) {
    setBackgroundColor(bgColor);
    setTextColor(textColor);
    buttonText   = text;
    horzMargin   = 20;
    vertMargin   = 20;
    this->height = height;
    this->width  = width;
    textDirty = true;
    textSrf = NULL;
}

ButtonLayer::ButtonLayer(std::string text, uint32_t bgColor, uint32_t
        textColor, uint16_t width, uint16_t height, uint8_t marginHorz, uint8_t
        marginVert) {
    setBackgroundColor(bgColor);
    setTextColor(textColor);
    buttonText   = text;
    horzMargin   = marginHorz;
    vertMargin   = marginVert;
    this->height = height;
    this->width  = width;
    textDirty = true;
    textSrf = NULL;
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
    if (textDirty) {
        // Build the text surface containing the given string.
        textSrf = sizeText(buttonText);
        textDirty = false;
    }
}

/* ------------------------------------------------------------------------------
 * draw - Given a surface, draw a button onto the surface.
 */
void ButtonLayer::draw(SDL_Surface* dst) {
    // If the surface is not created successfully.
    if (textSrf != NULL) {
        // Width of the button shading.  Anything other than 1 looks ugly.
        const uint8_t shadeWidth = 1;

        // Map Background Color
        uint8_t rC = (bgColor & 0xFF0000) >> 16;
        uint8_t gC = (bgColor & 0x00FF00) >>  8;
        uint8_t bC = (bgColor & 0x0000FF) >>  0;
        uint32_t pprBgColor = SDL_MapRGB(dst->format, rC, gC, bC);

        // Draw the Upper Shade Light Gray.
        uint32_t pprUShColor = SDL_MapRGB(dst->format, 0xAA, 0xAA, 0xAA);
        SDL_Rect fillRect = getRect();
        SDL_FillRect(dst, &fillRect, pprUShColor);

        // Draw the Lower Shade Dark Gray
        uint32_t pprLShColor = SDL_MapRGB(dst->format, 0x44, 0x44, 0x44);
        fillRect.x += shadeWidth;
        fillRect.y += shadeWidth;
        SDL_FillRect(dst, &fillRect, pprLShColor);

        // Draw the Background.
        fillRect.h -= shadeWidth;
        fillRect.w -= shadeWidth;
        SDL_FillRect(dst, &fillRect, pprBgColor);

        // Draw the text centered within the button, obeying the margin.
        SDL_Rect btRect = getRect();
        btRect.x += round((btRect.w - textSrf->w) / 2.0) + shadeWidth;
        btRect.y += round((btRect.h - textSrf->h) / 2.0) + shadeWidth;
        SDL_BlitSurface(textSrf, NULL, dst, &btRect);
    }

}

/* ------------------------------------------------------------------------------
 * getFont - Load a font for usage.  Once the font is loaded, keep it
 * statically within to eliminate a global and also prevent re-loading the same
 * font.
 */
TTF_Font* ButtonLayer::getFont(uint8_t size) const {
    // Store loaded font here.
    TTF_Font* font = NULL;

    // If the font hasn't been loaded, load it.
    if (font == NULL) {

        // Is there a way to find these fonts in the filesystem?
        font = TTF_OpenFont(FONTPATH, size);

    }

    // Return the font.
    return font;
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

/* ------------------------------------------------------------------------------
 * Perform binary searches to build a text surface which will fit into the
 * ButtonLayer horizontally and vertically.
 *
 * Return that text surface.
 *
 * Maintenance note: If implementing a mutator which affects text appearance,
 * be sure to set textDirty = true.
 */
SDL_Surface* ButtonLayer::sizeText(std::string text) const {
    // Don't resize the text if it's already been sized once.
    SDL_Surface* textSrf = NULL;
    if (textSrf != NULL) {
        return textSrf;
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
    int fillH = r.w - horzMargin;
    int fillV = r.h - vertMargin;

    // Calculate the largest font size which will fit the ButtonLayer surface
    // height-wise.
    while (lo < hi && h != fillV) {
        mi = ((hi - lo) / 2) + lo;

        TTF_Font* fnt = getFont(mi);
        TTF_SizeText(fnt, text.c_str(), &w, &h);
        TTF_CloseFont(fnt);

        if (h > fillV) {
            hi = mi - 1;
        } else if (h < fillV) {
            lo = mi + 1;
        }
    }

    // Calculate the largest font size which will fit the ButtonLayer surface
    // width-wise.
    lo = 1;
    hi = mi;
    while (lo < hi && w != fillH) {
        mi = ((hi - lo) / 2) + lo;

        TTF_Font* fnt = getFont(mi);
        TTF_SizeText(fnt, text.c_str(), &w, &h);
        TTF_CloseFont(fnt);

        if (w > fillH) {
            hi = mi - 1;
        } else if (w < fillH) {
            lo = mi + 1;
        }
    }

    // Render and return the text surface which fits in the ButtonLayer.
    TTF_Font* fnt = getFont(mi);
    uint8_t rC = (textColor & 0xFF0000) >> 16;
    uint8_t gC = (textColor & 0x00FF00) >>  8;
    uint8_t bC = (textColor & 0x0000FF) >>  0;
    SDL_Color color = {rC, gC, bC};
    textSrf = TTF_RenderText_Blended(fnt, text.c_str(), color);
    TTF_CloseFont(fnt);

    // Confirm that the text surface was created.  If not something is horribly
    // wrong, so die.
    if (textSrf == NULL) {
        std::cout << text << std::endl;
        std::cout << "Error drawing text: " << TTF_GetError() << std::endl;
        exit(2);
    }

    return textSrf;
}
