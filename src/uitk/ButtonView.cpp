#include <sstream>
#include "../uitk/ButtonView.hpp"

/**
 * Constructor.
 * <p>
 * Initializes default margin widths.
 * @param rect
 */
ButtonView::ButtonView(const SDL_Rect &rect) : BaseView(nullptr, rect) {
    horizontalMargin   = 20;
    verticalMargin   = 20;
    textDirty = true;
    textTexture = nullptr;
    icon = nullptr;
}

/**
 * Destructor.
 */
ButtonView::~ButtonView() {
    if (icon != nullptr) {
        delete icon;
    }
}

/**
 * Given the renderer, draw a button onto it.
 * @param renderer
 */
void ButtonView::draw(SDL_Renderer* renderer) {
    // Width of the button shading.  Anything other than 1 looks ugly.
    const uint8_t shadeWidth = 1;

    SDL_Rect fillRect = getRect();

    // Draw the Upper Shadow
    if (isPressed()) {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);

    } else {
        SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
    }
    SDL_RenderDrawRect(renderer, &fillRect);

    // Draw the Lower Shadow
    if (isPressed()) {
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
    SDL_RenderFillRect(renderer, &fillRect);

    if (textDirty) {
        // Build the text surface containing the given string.
        textTexture = sizeText(renderer, text);
        textDirty = false;
    }

    // Draw the text centered within the button, obeying the margin.
    // A nullptr textTexture implies the text is a 0-length string.
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
    } else if (icon != nullptr) {
        uint16_t x = (uint16_t) (fillRect.x + (fillRect.w - icon->getWidth()) / 2);
        uint16_t y = (uint16_t) (fillRect.y + (fillRect.h - icon->getHeight()) / 2);
        icon->move(x, y);
        icon->draw(renderer);
    }
}

/**
 * Load a font for usage.
 * @param size
 * @return TTF_Font*
 */
TTF_Font* ButtonView::getFont(uint8_t size) const {
    return TTF_OpenFont(fontPath.c_str(), size);
}

/**
 * Sets the background color of the button.
 * @param color
 */
void ButtonView::setBackgroundColor(uint32_t color) {
    bgColor = color;
}

/**
 * Sets the text color of the button.
 * @param color
 */
void ButtonView::setTextColor(uint32_t color) {
    textDirty = true;
    textColor = color;
}

/**
 * Sets the text on the button to the value of the provided string.
 */
void ButtonView::setText(std::string text) {
    textDirty = true;
    this->text = text;
}

/**
 * Change the height of the button to the provided height.
 */
void ButtonView::setHeight(uint16_t newHeight) {
    textDirty = true;
    BaseView::setHeight(newHeight);
}

/**
 * Change the width of the button to the provided width.
 */
void ButtonView::setWidth(uint16_t newWidth) {
    textDirty = true;
    BaseView::setWidth(newWidth);
}

/**
 * Move the button to a new X coordinate.
 * @param pX
 */
void ButtonView::setX(uint16_t newX) {
    textDirty = true;
    BaseView::setX(newX);
}

/**
 * Move the button to a new Y coordinate.
 * @param pX
 */
void ButtonView::setY(uint16_t newY) {
    textDirty = true;
    BaseView::setY(newY);
}

/**
 * Change the horizontal pixel width of padding between the edge of the button on the text within.
 * @param marginHorizontal
 */
void ButtonView::setMarginHorizontal(uint16_t marginHorizontal) {
    horizontalMargin = marginHorizontal;
}

/**
 * Change the vertical pixel width of padding between the edge of the button on the text within.
 * @param marginVertical
 */
void ButtonView::setMarginVertical(uint16_t marginVertical) {
    verticalMargin = marginVertical;
}

/**
 * Perform binary searches to build a text texture which will fit into the ButtonView horizontally and vertically.
 * <p>
 * Maintenance note: If implementing a mutator which affects text appearance, be sure to set textDirty = true.
 * @param renderer
 * @param text
 * @return SDL_Texture*
 */
SDL_Texture* ButtonView::sizeText(SDL_Renderer* renderer, std::string text) const {
    if (text.empty()) {
        return nullptr;
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

    // Calculate the largest font size which will fit the ButtonView surface
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

    // Calculate the largest font size which will fit the ButtonView surface
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

    // Render and return the text surface which fits in the ButtonView.
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

/**
 * Change the icon that appears on the button.
 * @param animation
 */
void ButtonView::setIcon(Animation* animation) {
    icon = animation;
}

/**
 * Change the font used for the text.
 * @param fontPath
 */
void ButtonView::setFontPath(std::string fontPath) {
    this->fontPath = fontPath;
}
