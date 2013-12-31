#ifndef BUTTONLAYER_HPP
#define BUTTONLAYER_HPP

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "KeyRunner.hpp"
#include "Layer.hpp"

class ButtonLayer : public Layer {
public:
    ButtonLayer(std::string text, uint32_t bgColor);

    virtual void draw(SDL_Surface* dst);
    virtual SDL_Rect getRect() const;
    virtual void update();

    void setBackgroundColor(uint32_t color);

private:
    void drawText(SDL_Surface* dst, std::string s) const;
    TTF_Font* getFont() const;

    std::string buttonText;

    uint32_t bgColor;
};

#endif//BUTTONLAYER_HPP
