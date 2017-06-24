#ifndef BUTTON_LAYER_HPP
#define BUTTON_LAYER_HPP

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../uitk/Animation.hpp"
#include "../uitk/Layer.hpp"

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
    void setMarginHorizontal(uint16_t marginHorizontal);
    void setMarginVertical(uint16_t marginVertical);
    void setIcon(Animation* animation);
    void setFontPath(std::string fontPath);

private:
    ButtonLayer();

    TTF_Font* getFont(uint8_t size) const;
    SDL_Texture* sizeText(SDL_Renderer* renderer, std::string text) const;

    std::string buttonText;

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

    friend class ButtonLayerBuilder;
};

#endif
