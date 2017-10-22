#include <sstream>
#include "../uitk/LabelView.hpp"

/**
 * Constructor.
 * @param model
 * @param rect
 */
LabelView::LabelView(Model* model, const SDL_Rect &rect) : RectangleView(model, rect) {
    horizontalMargin = 20;
    verticalMargin = 20;
    textDirty = true;
    textTexture = nullptr;
    icon = nullptr;
    fontSize = 0;
}

/**
 * Destructor.
 */
LabelView::~LabelView() {
    delete icon;
}

/**
 * Load a font for usage.
 * @param size
 * @return TTF_Font*
 */
TTF_Font* LabelView::getFont(uint8_t size) const {
    return TTF_OpenFont(fontPath.c_str(), size);
}

/**
 * Sets the text color of the label.
 * @param color
 */
void LabelView::setTextColor(uint32_t color) {
    textDirty = true;
    textColor = color;
}

/**
 * Sets the text on the label to the value of the provided string.
 */
void LabelView::setText(std::string text) {
    textDirty = true;
    this->text = std::move(text);
}

/**
 * Change the height of the label to the provided height.
 */
void LabelView::setHeight(uint16_t newHeight) {
    textDirty = true;
    BaseView::setHeight(newHeight);
}

/**
 * Change the width of the label to the provided width.
 */
void LabelView::setWidth(uint16_t newWidth) {
    textDirty = true;
    BaseView::setWidth(newWidth);
}

/**
 * Move the label to a new X coordinate.
 * @param pX
 */
void LabelView::setX(uint16_t newX) {
    textDirty = true;
    BaseView::setX(newX);
}

/**
 * Move the button to a new Y coordinate.
 * @param pX
 */
void LabelView::setY(uint16_t newY) {
    textDirty = true;
    BaseView::setY(newY);
}

/**
 * Change the horizontal pixel width of padding between the edge of the button on the text within.
 * @param marginHorizontal
 */
void LabelView::setMarginHorizontal(uint16_t marginHorizontal) {
    horizontalMargin = marginHorizontal;
}

/**
 * Change the vertical pixel width of padding between the edge of the button on the text within.
 * @param marginVertical
 */
void LabelView::setMarginVertical(uint16_t marginVertical) {
    verticalMargin = marginVertical;
}

/**
 * Builds a texture for the text.
 * <p>
 * When fontSize is 0, fit the text to the label's region. Otherwise, treat the font size as a fixed number.
 * <p>
 * Maintenance note: If implementing a mutator which affects text appearance, be sure to set textDirty = true.
 * @param renderer
 * @param text
 * @return SDL_Texture*
 */
SDL_Texture* LabelView::makeTextTexture(SDL_Renderer* renderer) const {
    if (text.empty()) {
        return nullptr;
    }

    uint32_t fontSize = this->getFontSize();
    if (fontSize == 0) {
        fontSize = this->chooseFontSizeToFit();
    }

    // Render and return the text surface which fits in the LabelView
    TTF_Font* fnt = getFont(static_cast<uint8_t>(fontSize));
    auto rC = static_cast<uint8_t>((textColor & 0xFF0000) >> 16);
    auto gC = static_cast<uint8_t>((textColor & 0x00FF00) >>  8);
    auto bC = static_cast<uint8_t>((textColor & 0x0000FF) >>  0);
    SDL_Color color = {rC, gC, bC};
    SDL_Surface* textSurface = TTF_RenderText_Solid(fnt, text.c_str(), color);
    TTF_CloseFont(fnt);

    // Confirm that the text surface was created.  If not something is horribly wrong, so die
    if (textSurface == nullptr) {
        std::stringstream message;
        message << "Error drawing text: " << TTF_GetError();
        throw std::runtime_error(message.str());
    }

    // Create and return a texture for this surface
    return SDL_CreateTextureFromSurface(renderer, textSurface);
}

/**
 * Choose a font size that will cause the label's text to fit horizontally and vertically within the center of the
 * label's viewable area.
 * <p>
 * This sizing is implemented as a binary search for efficiency.
 * @return uint32_t
 */
uint32_t LabelView::chooseFontSizeToFit() const {
    // Lo, Hi and Mid variables for the binary search
    uint8_t lo = 1;
    uint8_t hi = 255;
    uint8_t mi = lo;

    // Width and Height vars which will store the calculated width and height of rendered text surfaces
    int w = 0;
    int h = 1;

    // Determine the region of the button to fill with text based on the margin and button height/width
    int fillH = getWidth() - horizontalMargin;
    int fillV = getHeight() - verticalMargin;

    // Calculate the largest font size which will fit the LabelView surface height-wise
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

    // Calculate the largest font size which will fit the LabelView surface width-wise
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

    return mi;
}

/**
 * Change the icon that appears on the label.
 * @param animation
 */
void LabelView::setIcon(Animation* animation) {
    icon = animation;
}

/**
 * Change the font used for the text.
 * @param fontPath
 */
void LabelView::setFontPath(std::string fontPath) {
    this->fontPath = std::move(fontPath);
}

/**
 * Given the renderer, draw a label onto it.
 * @param renderer
 */
void LabelView::draw(SDL_Renderer* renderer) {
    RectangleView::draw(renderer);

    // Fetch the text. Subclasses are allowed to have set textDirty by indirectly calling setText()
    std::string text = getText();

    if (textDirty) {
        // Build the text surface containing the given string.
        textTexture = makeTextTexture(renderer);
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
        SDL_Rect destination = {0, 0, w, h};
        destination.x = getX() + (uint32_t) round((getWidth() - w) / 2.0);
        destination.y = getY() + (uint32_t) round((getHeight() - h) / 2.0);

        // Draw the text into that rectangle
        SDL_RenderCopy(renderer, textTexture, nullptr, &destination);

        // OR,
        // Draw the Icon, centered.
    } else if (icon != nullptr) {
        auto x = static_cast<uint16_t>(getX() + (getWidth() - icon->getWidth()) / 2);
        auto y = static_cast<uint16_t>(getY() + (getHeight() - icon->getHeight()) / 2);
        icon->move(x, y);
        icon->draw(renderer);
    }
}

/**
 * Get the text that appears on the label.
 * @return
 */
std::string LabelView::getText() {
    return this->text;
}

/**
 * Set the font size.
 * <p>
 * A font size of 0 will cause the text to be automatically sized to fit within the label.
 * @param fontSize
 */
void LabelView::setFontSize(uint32_t fontSize) {
    this->textDirty = true;
    this->fontSize = fontSize;
}

/**
 * Get the font size.
 * <p>
 * A font size of 0 indicates that the text will be automatically sized to fit within the label.
 * @return uint32_t
 */
uint32_t LabelView::getFontSize() const {
    return fontSize;
}

/**
 * Get the color of the text.
 * @return uint32_t
 */
uint32_t LabelView::getTextColor() const {
    return textColor;
}
