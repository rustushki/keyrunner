#ifndef BUTTONLAYER_HPP
#define BUTTONLAYER_HPP

#include <functional>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "KeyRunner.hpp"
#include "Layer.hpp"

class ButtonLayer : public Layer {
public:
    ButtonLayer(std::string text, uint32_t bgColor, uint32_t textColor, uint16_t width,
        uint16_t height);
    ButtonLayer(std::string text, uint32_t bgColor, uint32_t textColor, uint16_t width,
        uint16_t height, uint8_t marginHorz, uint8_t marginVert);

    virtual void draw(SDL_Surface* dst);
    virtual SDL_Rect getRect() const;
    virtual void update();

    void setBackgroundColor(uint32_t color);
    void setTextColor(uint32_t color);
    void setText(std::string text);
    void setHeight(uint16_t height);
    void setWidth(uint16_t newWidth);
    void setX(uint16_t pX);
    void setY(uint16_t pY);
    void setOnClick(std::function<void()> cb);

private:
    TTF_Font* getFont(uint8_t size) const;
    SDL_Surface* sizeText(std::string text) const;

    std::string buttonText;

    uint32_t bgColor;
    uint32_t textColor;
    uint8_t horzMargin;
    uint8_t vertMargin;
    uint16_t x;
    uint16_t y;
    uint16_t height;
    uint16_t width;
    bool textDirty;

    std::function<void()> onClickCb;

    SDL_Surface* textSrf;
};

#endif//BUTTONLAYER_HPP
