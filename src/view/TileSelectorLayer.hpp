#ifndef TILESELECTORLAYER_HPP
#define TILESELECTORLAYER_HPP

#include "../model/TileType.hpp"

class TileSelectorLayer : public Layer {
public:
    TileSelectorLayer();

    virtual void draw(SDL_Surface* screen);
    virtual SDL_Rect getRect() const;

    virtual void onSelected();

private:
    void onKeyDown(SDLKey key);
};

#endif//TILESELECTORLAYER_HPP
