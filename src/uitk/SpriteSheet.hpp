#ifndef SPRITE_SHEET_HPP
#define SPRITE_SHEET_HPP
#include <string>
#include <SDL2/SDL.h>

class SpriteSheet {

public:
    SpriteSheet(std::string filename, uint16_t width, uint16_t height);
    void blitFrame(SDL_Renderer* renderer, uint16_t x, uint16_t y, SDL_Rect& where) const;

    uint16_t getWidth() const;
    uint16_t getHeight() const;

private:
    uint16_t height;
    uint16_t width;

    SDL_Renderer* renderer;
    SDL_Surface* sheet;
};

#endif//SPRITE_SHEET_HPP
