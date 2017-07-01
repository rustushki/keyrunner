#include "../uitk/ButtonLayer.hpp"
#include "../uitk/ButtonLayerBuilder.hpp"

ButtonLayerBuilder::ButtonLayerBuilder() {
    buttonLayer = nullptr;
    resetIfNull();
}

void ButtonLayerBuilder::resetIfNull() {
    if (buttonLayer == nullptr) {
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

ButtonLayerBuilder* ButtonLayerBuilder::setMarginHorizontal(uint8_t marginHorizontal) {
    resetIfNull();
    buttonLayer->setMarginHorizontal(marginHorizontal);
    return this;
}

ButtonLayerBuilder* ButtonLayerBuilder::setMarginVertical(uint8_t marginVertical) {
    resetIfNull();
    buttonLayer->setMarginVertical(marginVertical);
    return this;
}

ButtonLayerBuilder* ButtonLayerBuilder::setIcon(Animation* animation) {
    resetIfNull();
    buttonLayer->setIcon(animation);
    return this;
}

ButtonLayerBuilder* ButtonLayerBuilder::setFontPath(std::string fontPath) {
    resetIfNull();
    buttonLayer->setFontPath(fontPath);
    return this;
}

ButtonLayer* ButtonLayerBuilder::build() {
    ButtonLayer* temp = buttonLayer;
    buttonLayer = nullptr;
    return temp;
}

