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
    r.x = 0;
    r.y = GridLayer::GetInstance()->getRect().h;
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
 * drawText - Given a surface and a string draw text to that surface, filling
 * the available space inside the button to proportion.
 *
 * TODO: mention the text color.
 */
void ButtonLayer::draw(SDL_Surface* dst) {
    // If the surface is not created successfully.
    if (textSrf != NULL) {
        // Get the background color in the same pixel format as the destination
        // surface.  Fill a rectangle to that color.
        SDL_Rect r = getRect();
        uint8_t rC = (bgColor & 0xFF0000) >> 16;
        uint8_t gC = (bgColor & 0x00FF00) >>  8;
        uint8_t bC = (bgColor & 0x0000FF) >>  0;
        uint32_t pprBgColor = SDL_MapRGB(dst->format, rC, gC, bC);
        SDL_FillRect(dst, &r, pprBgColor);

        // Center the text within the button, obeying the margin.
        r.x += horzMargin / 2;
        r.y += vertMargin / 2;
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
