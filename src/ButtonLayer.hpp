#ifndef BUTTONLAYER_HPP
#define BUTTONLAYER_HPP

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "KeyRunner.hpp"
#include "Layer.hpp"

class ButtonLayer : public Layer {
public:
    ButtonLayer(std::string text, uint32_t bgColor, uint16_t width, uint16_t height);
    ButtonLayer(std::string text, uint32_t bgColor, uint16_t width, uint16_t height,
        uint8_t marginHorz, uint8_t marginVert);

    virtual void draw(SDL_Surface* dst);
    virtual SDL_Rect getRect() const;
    virtual void update();

    void setBackgroundColor(uint32_t color);

private:
    void drawText(SDL_Surface* dst, std::string s) const;
    TTF_Font* getFont(uint8_t size) const;
    SDL_Surface* sizeText(std::string text) const;

    std::string buttonText;

    uint32_t bgColor;
    uint8_t horzMargin;
    uint8_t vertMargin;
    uint16_t height;
    uint16_t width;
};

#endif//BUTTONLAYER_HPP
