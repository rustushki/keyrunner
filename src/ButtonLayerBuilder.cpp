#include "ButtonLayer.hpp"
#include "ButtonLayerBuilder.hpp"

ButtonLayerBuilder::ButtonLayerBuilder() {
    buttonLayer = NULL;
    resetIfNull();
}

void ButtonLayerBuilder::resetIfNull() {
    if (buttonLayer == NULL) {
        buttonLayer = new ButtonLayer();
    }
}

ButtonLayerBuilder* ButtonLayerBuilder::setText(std::string text) {
    resetIfNull();
    buttonLayer->setText(text);
    return this;
}

ButtonLayerBuilder* ButtonLayerBuilder::setBgColor(uint32_t bgColor) {
    resetIfNull();
    buttonLayer->setBackgroundColor(bgColor);
    return this;
}

ButtonLayerBuilder* ButtonLayerBuilder::setTextColor(uint32_t textColor) {
    resetIfNull();
    buttonLayer->setTextColor(textColor);
    return this;
}

ButtonLayerBuilder* ButtonLayerBuilder::setWidth(uint16_t width) {
    resetIfNull();
    buttonLayer->setWidth(width);
    return this;
}

ButtonLayerBuilder* ButtonLayerBuilder::setHeight(uint16_t height) {
    resetIfNull();
    buttonLayer->setHeight(height);
    return this;
}

ButtonLayerBuilder* ButtonLayerBuilder::setX(uint16_t x) {
    resetIfNull();
    buttonLayer->setX(x);
    return this;
}

ButtonLayerBuilder* ButtonLayerBuilder::setY(uint16_t y) {
    resetIfNull();
    buttonLayer->setY(y);
    return this;
}

ButtonLayerBuilder* ButtonLayerBuilder::setMarginHorz(uint8_t marginHorz) {
    resetIfNull();
    buttonLayer->setMarginHorz(marginHorz);
    return this;
}

ButtonLayerBuilder* ButtonLayerBuilder::setMarginVert(uint8_t marginVert) {
    resetIfNull();
    buttonLayer->setMarginVert(marginVert);
    return this;
}

ButtonLayerBuilder* ButtonLayerBuilder::setIcon(AnimationType at) {
    resetIfNull();
    buttonLayer->setIcon(at);
    return this;
}

ButtonLayer* ButtonLayerBuilder::build() {
    ButtonLayer* temp = buttonLayer;
    buttonLayer = NULL;
    return temp;
}
