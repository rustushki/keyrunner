#ifndef BUTTONLAYERBUILDER_HPP
#define BUTTONLAYERBUILDER_HPP

#include "KeyRunner.hpp"

class ButtonLayer;

class ButtonLayerBuilder {
public:
    ButtonLayerBuilder();
    ButtonLayerBuilder* setText(std::string text);
    ButtonLayerBuilder* setBgColor(uint32_t bgColor);
    ButtonLayerBuilder* setTextColor(uint32_t textColor);
    ButtonLayerBuilder* setWidth(uint16_t width);
    ButtonLayerBuilder* setHeight(uint16_t height);
    ButtonLayerBuilder* setX(uint16_t x);
    ButtonLayerBuilder* setY(uint16_t y);
    ButtonLayerBuilder* setMarginHorz(uint8_t marginHorz);
    ButtonLayerBuilder* setMarginVert(uint8_t marginVert);
    ButtonLayer* build();
    void resetIfNull();

private:
    ButtonLayer* buttonLayer;
};

#endif//BUTTONLAYERBUILDER_HPP
