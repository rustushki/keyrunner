#include "ButtonLayer.hpp"
#include "GridLayer.hpp"

ButtonLayer::ButtonLayer(std::string text, uint32_t bgColor) {
    buttonText = text;
    setBackgroundColor(bgColor);
}

void ButtonLayer::draw(SDL_Surface* dst) {
    drawText(dst, buttonText);
}

SDL_Rect ButtonLayer::getRect() const {
    SDL_Rect r;
    r.x = 0;
    r.y = GridLayer::GetInstance()->getRect().h;
    r.w = 50;
    r.h = 30;
    return r;
}

void ButtonLayer::update() {
}

/* ------------------------------------------------------------------------------
 * drawText - Given a surface and a string draw text to that surface, filling
 * the available space inside the button to proportion.
 *
 * TODO: mention the text color.
 */
void ButtonLayer::drawText(SDL_Surface* dst, std::string s) const {
    // Build the text surface containing the given string.
    SDL_Surface* textSrf = sizeText(s);

    // If the surface is not created successfully.
    if (textSrf == NULL) {
        std::cout << s.c_str() << std::endl;
        std::cout << "Error creating text: " << TTF_GetError() << std::endl;
        exit(2);

    // Otherwise,
    } else {
        // Blit the text to the screen.
        SDL_Rect r = getRect();

        // Get the background color in the same pixel format as the destination
        // surface.  Fill a rectangle to that color.
        uint8_t rC = (bgColor & 0xFF0000) >> 16;
        uint8_t gC = (bgColor & 0x00FF00) >>  8;
        uint8_t bC = (bgColor & 0x0000FF) >>  0;
        uint32_t pprBgColor = SDL_MapRGB(dst->format, rC, gC, bC);
        SDL_FillRect(dst, &r, pprBgColor);

        SDL_BlitSurface(textSrf, NULL, dst, &r);
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

/* ------------------------------------------------------------------------------
 * Perform binary searches to build a text surface which will fit into the
 * ButtonLayer horizontally and vertically.
 *
 * Return that text surface.
 */
SDL_Surface* ButtonLayer::sizeText(std::string text) const {
    // Rectangle representing the size of the ButtonLayer.
    SDL_Rect r = getRect();

    // Lo, Hi and Mid variables for the binary search.
    uint8_t lo = 1;
    uint8_t hi = 255;
    uint8_t mi = lo;

    // Width and Height vars which will store the calculated width and height
    // of rendered text surfaces.
    int w = 0;
    int h = 1;

    // Calculate the largest font size which will fit the ButtonLayer surface
    // height-wise.
    while (lo < hi && h != r.h) {
        mi = ((hi - lo) / 2) + lo;

        TTF_Font* fnt = getFont(mi);
        TTF_SizeText(fnt, text.c_str(), &w, &h);
        TTF_CloseFont(fnt);

        if (h > r.h) {
            hi = mi - 1;
        } else if (h < r.h) {
            lo = mi + 1;
        }
    }

    // Calculate the largest font size which will fit the ButtonLayer surface
    // width-wise.
    lo = 1;
    hi = mi;
    while (lo < hi && w != r.w) {
        mi = ((hi - lo) / 2) + lo;

        TTF_Font* fnt = getFont(mi);
        TTF_SizeText(fnt, text.c_str(), &w, &h);
        TTF_CloseFont(fnt);

        if (w > r.w) {
            hi = mi - 1;
        } else if (w < r.w) {
            lo = mi + 1;
        }
    }

    // Render and return the text surface which fits in the ButtonLayer.
    TTF_Font* fnt = getFont(mi);
    SDL_Color color = {0xAA, 0xAA, 0xAA};
    SDL_Surface* textSrf = TTF_RenderText_Blended(fnt, text.c_str(), color);
    TTF_CloseFont(fnt);
    return textSrf;
}
