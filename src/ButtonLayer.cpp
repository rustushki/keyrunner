#include "ButtonLayer.hpp"
#include "GridLayer.hpp"

ButtonLayer::ButtonLayer(std::string text) {
    buttonText = text;
}

void ButtonLayer::draw(SDL_Surface* dst) {
    drawText(dst, buttonText);
}

SDL_Rect ButtonLayer::getRect() const {
    SDL_Rect r;
    r.x = 0;
    r.y = GridLayer::GetInstance()->getRect().h;
    r.w = 30;
    r.h = 20;
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
    // Gray
    SDL_Color color = {0xAA, 0xAA, 0xAA};

    // Build the text surface containing the given string.
    SDL_Surface* text_surface = TTF_RenderText_Solid(getFont(), s.c_str(), color);

    // If the surface is not created successfully.
    if (text_surface == NULL) {
        std::cout << s.c_str() << std::endl;
        std::cout << "Error creating text: " << TTF_GetError() << std::endl;
        exit(2);

    // Otherwise,
    } else {
        // Blit the text to the screen.
        SDL_Rect r = getRect();
        SDL_BlitSurface(text_surface, NULL, dst, &r);
    }

}

/* ------------------------------------------------------------------------------
 * getFont - Load a font for usage.  Once the font is loaded, keep it
 * statically within to eliminate a global and also prevent re-loading the same
 * font.
 */
TTF_Font* ButtonLayer::getFont() const {
    // Store loaded font here.
    static TTF_Font* font = NULL;

    // If the font hasn't been loaded, load it.
    if (font == NULL) {

        // Is there a way to find these fonts in the filesystem?
        font = TTF_OpenFont(FONTPATH, 25);

    }

    // Return the font.
    return font;
}
