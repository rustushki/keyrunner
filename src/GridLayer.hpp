#ifndef GLIDLAYER_HPP
#define GLIDLAYER_HPP

#include <vector>

#include "Layer.hpp"

class TileLayer;

class GridLayer : public Layer {

public:

    static GridLayer* GetInstance();

    static const uint16_t GRID_HEIGHT = 16;
    static const uint16_t GRID_WIDTH  = 25;

    void pushAnimatedTile(TileLayer* tile);
    void clearAnimatedTiles();
    void animateTiles();
    void clearChangedTiles();
    void addChangedTile(TileLayer* tile);

    virtual void draw(SDL_Surface* dst);
    virtual SDL_Rect getRect() const;
    virtual void update();

private:

    static GridLayer* instance;

    GridLayer();

    std::vector<TileLayer*> changedTiles;
    std::vector<TileLayer*> animatedTiles;

};

#endif//GLIDLAYER_HPP
