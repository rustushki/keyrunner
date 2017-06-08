#ifndef BUTTONLAYER_HPP
#define BUTTONLAYER_HPP

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Animation.hpp"
#include "Layer.hpp"

class ButtonLayer : public Layer {
public:
    ~ButtonLayer();

    virtual void draw(SDL_Renderer* renderer);
    virtual SDL_Rect getRect() const;
    virtual void update();

    void setBackgroundColor(uint32_t color);
    void setTextColor(uint32_t color);
    void setText(std::string text);
    void setHeight(uint16_t height);
    void setWidth(uint16_t newWidth);
    void setX(uint16_t pX);
    void setY(uint16_t pY);
    void setMarginHorz(uint8_t marginHorz);
    void setMarginVert(uint16_t marginVert);
    void setIcon(Animation* animation);
    void setFontPath(std::string fontPath);

private:
    ButtonLayer();

    TTF_Font* getFont(uint8_t size) const;
    SDL_Texture* sizeText(SDL_Renderer* renderer, std::string text) const;

    std::string buttonText;

    uint32_t bgColor;
    uint32_t textColor;
    uint8_t horzMargin;
    uint8_t vertMargin;
    uint16_t x;
    uint16_t y;
    uint16_t height;
    uint16_t width;
    Animation* icon;
    std::string fontPath;
    bool textDirty;

    SDL_Texture* textTexture;

    friend class ButtonLayerBuilder;
};

#endif//BUTTONLAYER_HPP
