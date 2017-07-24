#include "../uitk/MenuView.hpp"
#include "../controller/KeyRunner.hpp"

/**
 * Constructor.
 * @param model
 * @param rect
 */
MenuView::MenuView(Model *model, const SDL_Rect &rect) : RectangleView(model, rect) {
    setCursorIndex(0);
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
    button->show();
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
}

/**
 * Resize the option buttons so that they fit vertically within the MenuView.
 */
void MenuView::sizeButtons() {
    int buttonIndex = 0;
    for (ButtonView* button : buttons) {
        button->setX(getX());
        button->setWidth(getWidth());
        button->setHeight(static_cast<uint16_t>(getHeight() / (buttons.size() + 1)));
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
