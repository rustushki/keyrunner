#include "../uitk/MenuView.hpp"
#include "../controller/KeyRunner.hpp"

/**
 * Constructor.
 * @param model
 * @param rect
 */
MenuView::MenuView(Model *model, const SDL_Rect &rect) : RectangleView(model, rect) {}

void MenuView::addOption(std::string optionText, const std::function<void(SDL_Event)> &callBack) {
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
    ButtonView* button = new ButtonView(nullptr, rect);
    button->setText(optionText);
    button->setFontPath(FONT_PATH);
    button->setFontSize(0);
    button->setOnMouseDownCallback(callBack);
    button->setColor(getOptionBackgroundColor());
    button->setTextColor(getOptionTextColor());
    button->show();
    buttons.push_back(button);

    sizeButtons();

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
