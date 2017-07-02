#ifndef BUTTON_VIEW_HPP
#define BUTTON_VIEW_HPP

#include <SDL_ttf.h>
#include "../uitk/BaseView.hpp"

class ButtonView : public BaseView {
public:
    ButtonView(const SDL_Rect &rect);
    virtual ~ButtonView() override;
    void draw(SDL_Renderer* renderer) override;

    void setBackgroundColor(uint32_t color);
    void setTextColor(uint32_t color);
    void setText(std::string text);
    void setHeight(uint16_t height);
    void setWidth(uint16_t newWidth);
    void setX(uint16_t pX);
    void setY(uint16_t pY);
    void setMarginHorizontal(uint16_t marginHorizontal);
    void setMarginVertical(uint16_t marginVertical);
    void setIcon(Animation* animation);
    void setFontPath(std::string fontPath);

private:
    TTF_Font* getFont(uint8_t size) const;
    SDL_Texture* sizeText(SDL_Renderer* renderer, std::string text) const;

    std::string text;
    uint32_t bgColor;
    uint32_t textColor;
    uint16_t horizontalMargin;
    uint16_t verticalMargin;
    uint16_t x;
    uint16_t y;
    uint16_t height;
    uint16_t width;
    Animation* icon;
    std::string fontPath;
    bool textDirty;
    SDL_Texture* textTexture;
};

#endif