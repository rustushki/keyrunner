#include "../uitk/MenuView.hpp"
#include "../controller/KeyRunner.hpp"

/**
 * Constructor.
 * @param model
 * @param rect
 */
MenuView::MenuView(Model* model, const SDL_Rect &rect) : RectangleView(model, rect) {
    setCursorIndex(0);
    setVisibleOptionCount(0);
    setWindowTopIndex(0);
    setOptionFontSize(0);
    createArrows();
    cursorTextColor = 0;
}

void MenuView::addOption(std::string optionText, const std::function<void(SDL_Event)> &callBack) {
    SDL_Rect rect = {0, 0, 0, 0};
    auto button = new ButtonView(nullptr, rect);
    button->setText(optionText);
    button->setFontPath(FONT_CELTIC_HAND);
    button->setFontSize(0);
    button->setOnMouseUpCallback(callBack);
    button->setColor(getOptionBackgroundColor());
    button->setTextColor(getOptionTextColor());
    button->setFontSize(getOptionFontSize());
    buttons.push_back(button);

    sizeButtons();
    setCursorIndex(getCursorIndex());

}

/**
 * Draw each of the visible buttons on the menu.
 * @param renderer
 */
void MenuView::draw(SDL_Renderer* renderer) {
    for (ButtonView* button : buttons) {
        if (button->isVisible()) {
            button->draw(renderer);
        }
    }

    if (!cantScrollUp()) {
        upArrowView->draw(renderer);
    }

    if (!cantScrollDown()) {
        downArrowView->draw(renderer);
    }
}

/**
 * Resize the option buttons so that they fit vertically within the MenuView.
 * <p>
 * Only take this action for buttons that fall within the current visible window of option.
 * <p>
 * If the visible option count is set to 0, it will fit all options into the area. Otherwise, it will fit the visible
 * option count of options into the area.
 */
void MenuView::sizeButtons() {
    if (!buttons.empty()) {
        for (auto button : buttons) {
            button->hide();
        }

        auto windowSize = getWindowSize();
        auto slotIndex = 0;
        for (auto optionIndex = getWindowTopIndex(); optionIndex < getWindowTopIndex() + windowSize; optionIndex++) {
            if (optionIndex < buttons.size()) {
                auto button = buttons[optionIndex];
                // Button X is the same as the MenuView X
                button->setX(getX());

                // Button Width is the same as the MenuView Width
                button->setWidth(getWidth());

                // Button Height is the MenuView height less the two arrows divided by the window size
                uint16_t buttonHeight = getHeight();
                buttonHeight -= upArrowView->getHeight();
                buttonHeight -= downArrowView->getHeight();
                buttonHeight /= windowSize;
                button->setHeight(buttonHeight);

                // Button Y is relative to the bottom edge of the up arrow button
                uint16_t buttonY = getY();
                buttonY += upArrowView->getHeight();
                buttonY += slotIndex * button->getHeight();
                button->setY(buttonY);

                button->show();
                slotIndex++;
            }
        }
    }
}

/**
 * Set the background color of each of the menu options.
 * @param color
 */
void MenuView::setOptionBackgroundColor(uint32_t color) {
    this->optionBackgroundColor = color;
    for (ButtonView* button : buttons) {
        button->setColor(optionBackgroundColor);
    }
}

/**
 * Set the text color of each of the menu options.
 * @param color
 */
void MenuView::setOptionTextColor(uint32_t color) {
    this->optionTextColor = color;
    for (ButtonView* button : buttons) {
        button->setTextColor(optionTextColor);
    }
}

/**
 * Get the background color of each of the menu options.
 * @return
 */
uint32_t MenuView::getOptionBackgroundColor() {
    return optionBackgroundColor;
}

/**
 * Get the text color of each of the menu options.
 * @return
 */
uint32_t MenuView::getOptionTextColor() {
    return optionTextColor;
}

/**
 * Set the cursor when the mouse hovers over an option.
 * <p>
 * Afterwards, perform any user defined hover actions.
 * @param event
 */
void MenuView::onMouseHover(SDL_Event event) {
    uint16_t buttonIndex = 0;
    auto x = static_cast<uint16_t>(event.motion.x);
    auto y = static_cast<uint16_t>(event.motion.y);
    for (ButtonView* button : buttons) {
        if (button->containsPoint(x, y)) {
            setCursorIndex(buttonIndex);
            break;
        }

        buttonIndex++;
    }

    BaseView::onMouseHover(event);
}

/**
 * Set the cursor index, changing the color of that option and resetting the previously set option's color.
 * <p>
 * The option list starts at index 0.
 * @param newCursorIndex
 */
void MenuView::setCursorIndex(uint16_t newCursorIndex) {
    if (!buttons.empty()) {
        buttons[getCursorIndex()]->setTextColor(getOptionTextColor());
    }

    this->cursorIndex = newCursorIndex;

    if (!buttons.empty()) {
        buttons[getCursorIndex()]->setTextColor(cursorTextColor);
    }
}

/**
 * Get the cursor index.
 * <p>
 * The option list starts at index 0.
 * @return
 */
uint16_t MenuView::getCursorIndex() const {
    return cursorIndex;
}

/**
 * Set the color of the option on which the cursor is currently pointing.
 * @param color
 */
void MenuView::setOptionCursorTextColor(uint32_t color) {
    cursorTextColor = color;
}

/**
 * Move the cursor to the next option, wrapping around if on last.
 * <p>
 * If incrementing the cursor causes the cursor to fall outside of the visible window of options, also increment the
 * visible window of options.
 */
void MenuView::incrementCursor() {
    long newCursorIndex = getCursorIndex() + 1;
    if (newCursorIndex >= buttons.size()) {
        newCursorIndex = 0;
    }

    setCursorIndex(static_cast<uint16_t>(newCursorIndex));

    if (isCursorOutsideWindow()) {
        incrementWindowTopIndex();
    }

    sizeButtons();
}

/**
 * Move the cursor to the previous option, wrapping around if on first.
 * <p>
 * If decrementing the cursor causes the cursor to fall outside of the visible window of options, also decrement the
 * visible window of options.
 */
void MenuView::decrementCursor() {
    long newCursorIndex = getCursorIndex() - 1;
    if (newCursorIndex < 0) {
        newCursorIndex = buttons.size() - 1;
    }

    setCursorIndex(static_cast<uint16_t>(newCursorIndex));

    if (isCursorOutsideWindow()) {
        decrementWindowTopIndex();
    }

    sizeButtons();
}

/**
 * Delegates the mouse up event to the buttons that represent the options of the menu.
 * <p>
 * Searches for a button that contains the mouse click coordinate. Will then call that button's mouse up callback.
 * @param event
 */
void MenuView::onMouseUp(SDL_Event event) {
    auto x = static_cast<uint32_t>(event.button.x);
    auto y = static_cast<uint32_t>(event.button.y);

    for (ButtonView* buttonView : buttons) {
        if (buttonView->containsPoint(x, y)) {
            buttonView->onMouseUp(event);
        }
    }

    if (upArrowView->containsPoint(x, y)) {
        upArrowView->onMouseUp(event);
    }

    if (downArrowView->containsPoint(x, y)) {
        downArrowView->onMouseUp(event);
    }

    BaseView::onMouseUp(event);
}

/**
 * Delegates the key up event to the buttons that represent the options of the menu.
 * <p>
 * Internally:
 *  Enter - invokes the mouse up callback for the button that is the current cursor position
 *  Arrow Up - Move decrements the cursor
 *  Arrow Down - Increments the cursor
 * @param event
 */
void MenuView::onKeyUp(SDL_Event event) {
    // Enter activates the internal button
    if (event.key.keysym.sym == SDLK_RETURN) {
        buttons[getCursorIndex()]->onMouseUp(event);

    // Decrement the Cursor on UP
    } else if (event.key.keysym.sym == SDLK_UP) {
        decrementCursor();

    // Increment the Cursor on DOWN
    } else if (event.key.keysym.sym == SDLK_DOWN) {
        incrementCursor();
    }

    BaseView::onKeyUp(event);
}

/**
 * Sets the count of visible options.
 * <p>
 * If 0, it will attempt to display all options on the menu. Options not displayed may still be accessed.
 * <p>
 * This triggers the options to be re-sized to fit inside the area of the menu.
 * @param count
 */
void MenuView::setVisibleOptionCount(uint16_t count) {
    visibleOptionCount = count;
    sizeButtons();
}

/**
 * Get the count of visible options.
 * @return uint16_t
 */
uint16_t MenuView::getVisibleOptionCount() const {
    return visibleOptionCount;
}

/**
 * Create the up and down arrays of the menu.
 */
void MenuView::createArrows() {
    const auto arrowHeight = 5;

    SDL_Rect upRect = {getX(), getY(), getWidth(), arrowHeight};
    upArrowView = new ImageView(nullptr, upRect, ANIMATION_TYPE_ARROW_UP);
    upArrowView->setColor(0x000000);
    upArrowView->setHorizontalAlignment(HorizontalAlignment::CENTER);
    upArrowView->setVerticalAlignment(VerticalAlignment::CENTER);
    upArrowView->setOnMouseUpCallback([this](SDL_Event event) {
        decrementCursor();
    });


    SDL_Rect downRect = {getX(), getY() + getHeight() - arrowHeight, getWidth(), arrowHeight};
    downArrowView = new ImageView(nullptr, downRect, ANIMATION_TYPE_ARROW_DOWN);
    downArrowView->setColor(0x000000);
    downArrowView->setHorizontalAlignment(HorizontalAlignment::CENTER);
    downArrowView->setVerticalAlignment(VerticalAlignment::CENTER);
    downArrowView->setOnMouseUpCallback([this](SDL_Event event) {
        incrementCursor();
    });
}

/**
 * Returns true if scrolling isn't possible downwards.
 * @return boolean
 */
bool MenuView::cantScrollDown() const {
    return isScrollingEverNecessary() && (getWindowTopIndex() + 1 > buttons.size() - getWindowSize());
}

/**
 * Returns true if scrolling isn't possible upwards.
 * @return boolean
 */
bool MenuView::cantScrollUp() const {
    return isScrollingEverNecessary() && (getWindowTopIndex() - 1 < 0);
}

/**
 * Returns true if the count of items exceeds the number of visible items at a time.
 * <p>
 * The only exception is if the count of visible items is 0. In this case, all items will be displayed and fit within
 * the vertical space allotted.
 * @return boolean
 */
bool MenuView::isScrollingEverNecessary() const {
    bool isScrollingEverNecessary = true;

    // If the count of visible options is 0 or less, then all options will be fit into the area
    if (getVisibleOptionCount() <= 0) {
        isScrollingEverNecessary = false;
    }

    // If the count of visible options is more than the options available
    if (getWindowSize() >= buttons.size()) {
        isScrollingEverNecessary = false;
    }

    return isScrollingEverNecessary;
}

/**
 * Set the top index of the visible window of options.
 * <p>
 * This index denotes the first visible option to be displayed on the menu.
 * @param index
 */
void MenuView::setWindowTopIndex(uint16_t index) {
    this->windowTopIndex = index;
}

/**
 * Decrement the top index of the visible window of options.
 * <p>
 * If decrementing causes the top index to be negative, wrap around to the last position of the menu which still has a
 * full window-size worth of options remaining.
 */
void MenuView::decrementWindowTopIndex() {
    if (isScrollingEverNecessary()) {
        long newWindowTopIndex = 0;
        if (cantScrollUp()) {
            newWindowTopIndex = buttons.size() - getWindowSize();
        } else {
            newWindowTopIndex = getWindowTopIndex() - 1;
        }

        setWindowTopIndex(static_cast<uint16_t>(newWindowTopIndex));
    }
}

/**
 * Increment the top index of the visible window of options.
 * <p>
 * If incrementing causes the top index to fall after the last option in the option list, then wrap around to the first
 * option in the option list.
 */
void MenuView::incrementWindowTopIndex() {
    if (isScrollingEverNecessary()) {
        long newWindowTopIndex = 0;
        if (cantScrollDown()) {
            newWindowTopIndex = 0;
        } else {
            newWindowTopIndex = getWindowTopIndex() + 1;
        }

        setWindowTopIndex(static_cast<uint16_t>(newWindowTopIndex));
    }
}

/**
 * Get the top index of the visible window of options.
 * <p>
 * This index denotes the first visible option to be displayed on the menu.
 * @return uint16_t
 */
uint16_t MenuView::getWindowTopIndex() const {
    return windowTopIndex;
}

/**
 * Determine if the cursor is currently outside of the visible options.
 * @return boolean
 */
bool MenuView::isCursorOutsideWindow() {
    return getCursorIndex() < getWindowTopIndex() || getCursorIndex() >= getWindowTopIndex() + getWindowSize();
}

/**
 * Return the actual count of visible options.
 * <p>
 * Note that the visibleOptionCount could be 0. If this is the case, then this function will return the count of all the
 * options that could be displayed.
 * @return
 */
uint16_t MenuView::getWindowSize() const {
    long windowSize = getVisibleOptionCount();
    if (windowSize == 0) {
        windowSize = buttons.size();
    }
    return static_cast<uint16_t>(windowSize);
}

/**
 * Set the font size of each of the internal buttons inside the menu.
 * <p>
 * 0 means that the text of each button will be procedurally fit into the height and width of the button. Be careful
 * because this can lead to each button having irregularly sized text (unless all text strings have the same width).
 * @param optionFontSize
 */
void MenuView::setOptionFontSize(uint8_t optionFontSize) {
    this->optionFontSize = optionFontSize;

    for (auto button : buttons) {
        button->setFontSize(this->optionFontSize);
    }
}

/**
 * Get the font size of each of the internal buttons inside the menu.
 * @return uint8_t
 */
uint8_t MenuView::getOptionFontSize() const {
    return optionFontSize;
}
