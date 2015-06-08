#ifndef TILESELECTORLAYER_HPP
#define TILESELECTORLAYER_HPP

#include "../game/TileType.hpp"

class TileSelectorLayer : public Layer {
public:
    TileSelectorLayer();

    virtual void draw(SDL_Surface* screen);
    virtual SDL_Rect getRect() const;

private:
    void onKeyDown(SDLKey key);

    TileType selTileType;
};

#endif//TILESELECTORLAYER_HPP
