#include "../model/ButtonModel.hpp"

/**
 * Constructor.
 * @param text
 */
ButtonModel::ButtonModel(std::string text) {
    this->text = text;
    this->clicked = false;
}

/**
 * Get the text string that appears on the button.
 * @return string
 */
std::string ButtonModel::getText() {
    return text;
}

/**
 * Set whether the button is currently in the state of being clicked.
 * @param clicked
 */
void ButtonModel::setClicked(bool clicked) {
    this->clicked = clicked;
}

/**
 * Return whether the button is currently in the state of being clicked.
 * @return
 */
bool ButtonModel::isClicked() {
    return clicked;
}
