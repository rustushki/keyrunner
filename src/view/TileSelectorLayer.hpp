#ifndef TILESELECTORLAYER_HPP
#define TILESELECTORLAYER_HPP

#include "../model/TileType.hpp"

class TileSelectorLayer : public Layer {
public:
    TileSelectorLayer();

    virtual void draw(SDL_Renderer* renderer);
    virtual SDL_Rect getRect() const;

    virtual void onSelected();

private:
    void onKeyDown(SDL_Keycode key);
};

#endif//TILESELECTORLAYER_HPP
