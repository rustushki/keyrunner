#ifndef SPRITE_SHEET_HPP
#define SPRITE_SHEET_HPP
#include <string>
#include <SDL2/SDL.h>

class SpriteSheet {

public:
    SpriteSheet(SDL_Renderer* renderer, std::string filename, uint16_t width, uint16_t height);
    ~SpriteSheet();
    void drawStill(SDL_Renderer* renderer, uint16_t stillX, uint16_t stillY, SDL_Rect &where);

    uint16_t getWidth() const;
    uint16_t getHeight() const;

private:
    uint16_t height;
    uint16_t width;

    SDL_Texture* texture;
};

#endif
