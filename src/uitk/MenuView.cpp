#include "../uitk/MenuView.hpp"
#include "../controller/KeyRunner.hpp"

/**
 * Constructor.
 * @param model
 * @param rect
 */
MenuView::MenuView(Model *model, const SDL_Rect &rect) : RectangleView(model, rect) {
    setCursorIndex(0);
    setVisibleOptionCount(0);
    createArrows();
    cursorTextColor = 0;
}

void MenuView::addOption(std::string optionText, const std::function<void(SDL_Event)> &callBack) {
    SDL_Rect rect = {0, 0, 0, 0};
    auto button = new ButtonView(nullptr, rect);
    button->setText(optionText);
    button->setFontPath(FONT_PATH);
    button->setFontSize(0);
    button->setOnMouseUpCallback(callBack);
    button->setColor(getOptionBackgroundColor());
    button->setTextColor(getOptionTextColor());
    buttons.push_back(button);

    sizeButtons();
    setCursorIndex(getCursorIndex());

}

/**
 * Draw each of the buttons on the menu.
 * @param renderer
 */
void MenuView::draw(SDL_Renderer *renderer) {
    for (ButtonView* button : buttons) {
        button->draw(renderer);
    }

    if (canScrollUp()) {
        upArrowView->draw(renderer);
    }

    if (canScrollDown()) {
        downArrowView->draw(renderer);
    }
}

/**
 * Resize the option buttons so that they fit vertically within the MenuView.
 * <p>
 * If the visible option count is set to 0, it will fit all options into the area. Otherwise, it will fit the visible
 * option count of options into the area.
 */
void MenuView::sizeButtons() {
    uint16_t visibleOptions = getVisibleOptionCount();
    if (visibleOptions == 0) {
        visibleOptions = static_cast<uint16_t>(buttons.size());
    }

    int buttonIndex = 0;
    for (ButtonView* button : buttons) {
        button->setX(getX());
        button->setWidth(getWidth());
        button->setHeight(static_cast<uint16_t>(getHeight() / visibleOptions));
        button->setY(static_cast<uint16_t>(getY() + buttonIndex * button->getHeight()));
        buttonIndex++;
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
 */
void MenuView::incrementCursor() {
    long newCursorIndex = getCursorIndex() + 1;
    if (newCursorIndex >= buttons.size()) {
        newCursorIndex = 0;
    }
    setCursorIndex(static_cast<uint16_t>(newCursorIndex));
}

/**
 * Move the cursor to the previous option, wrapping around if on first.
 */
void MenuView::decrementCursor() {
    long newCursorIndex = getCursorIndex() - 1;
    if (newCursorIndex < 0) {
        newCursorIndex = buttons.size() - 1;
    }
    setCursorIndex(static_cast<uint16_t>(newCursorIndex));
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

    BaseView::onMouseUp(event);
}

/**
 * Delegates the key up event to the buttons that represent the options of the menu.
 * <p>
 * Internally, this invokes the mouse up callback for the button that is the current cursor position.
 * @param event
 */
void MenuView::onKeyUp(SDL_Event event) {
    if (event.key.keysym.sym == SDLK_RETURN) {
        buttons[getCursorIndex()]->onMouseUp(event);
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
    upArrowView = new RectangleView(nullptr, upRect);
    upArrowView->setColor(0xFF0000);

    SDL_Rect downRect = {getX(), getY() + getHeight() - arrowHeight, getWidth(), arrowHeight};
    downArrowView = new RectangleView(nullptr, downRect);
    downArrowView->setColor(0x00FF00);
}

/**
 * Returns true if scrolling is possible downwards.
 * <p>
 * Currently, this is set to always return true if scrolling is ever necessary.
 * @return boolean
 */
bool MenuView::canScrollDown() const {
    // Check if scrolling is ever necessary
    return isScrollingEverNecessary();
}

/**
 * Returns true if scrolling is possible upwards.
 * <p>
 * Currently, this is set to always return true if scrolling is ever necessary.
 * @return boolean
 */
bool MenuView::canScrollUp() const {
    return isScrollingEverNecessary();
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
    if (getVisibleOptionCount() >= buttons.size()) {
        isScrollingEverNecessary = false;
    }

    return isScrollingEverNecessary;
}

