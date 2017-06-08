#ifndef TILE_HPP
#define TILE_HPP

#include <vector>
#include "AnimationType.hpp"
#include "../model/Direction.hpp"
#include "../model/TileType.hpp"
#include "../uitk/Animation.hpp"
#include "../uitk/Layer.hpp"

class TileLayer : public Layer {

public:
    TileLayer(TileType tileType, uint16_t x, uint16_t y);
    ~TileLayer();

    static const uint8_t SIZE = 25;

    SDL_Rect getRect() const;

    void draw(SDL_Renderer* renderer, SDL_Texture* destination);

    Animation* getAnimation() const;

    TileType getType() const;
    void setType(TileType tt);

    uint16_t getX() const;
    uint16_t getY() const;

private:
    TileType tileType;
    Animation* animation;

    uint16_t x;
    uint16_t y;

};

#endif//TILE_HPP
