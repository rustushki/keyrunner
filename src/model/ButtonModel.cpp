#include "../model/ButtonModel.hpp"

ButtonModel::ButtonModel(std::string text) {
    this->text = text;
    this->clicked = false;
}

std::string ButtonModel::getText() {
    return text;
}

void ButtonModel::setClicked(bool clicked) {
    this->clicked = clicked;
}

bool ButtonModel::isClicked() {
    return clicked;
}
