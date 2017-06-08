#ifndef BUTTON_LAYER_BUILDER_HPP
#define BUTTON_LAYER_BUILDER_HPP

class Animation;
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
    ButtonLayerBuilder* setMarginHorizontal(uint8_t marginHorizontal);
    ButtonLayerBuilder* setMarginVertical(uint8_t marginVertical);
    ButtonLayerBuilder* setIcon(Animation* animation);
    ButtonLayerBuilder* setFontPath(std::string fontPath);
    ButtonLayer* build();
    void resetIfNull();

private:
    ButtonLayer* buttonLayer;
};

#endif
