#ifndef TILESELECTORLAYER_HPP
#define TILESELECTORLAYER_HPP

#include "Level.hpp"

class TileSelectorLayer : public Layer {
public:
    TileSelectorLayer();

    virtual void draw(SDL_Surface* screen);
    virtual SDL_Rect getRect() const;
    virtual void show();

private:
    void onTileTypeClick(TileType tileType);
    void onKeyDown(SDLKey key);

    TileType selTileType;
};

#endif//TILESELECTORLAYER_HPP
