#ifndef GLIDLAYER_HPP
#define GLIDLAYER_HPP

#include <vector>

#include "Layer.hpp"

class Tile;

class GridLayer : public Layer {

public:

    static GridLayer* GetInstance();

    static const uint16_t GRID_HEIGHT = 16;
    static const uint16_t GRID_WIDTH  = 25;

    void pushAnimatedTile(Tile* tile);
    void clearAnimatedTiles();
    void animateTiles();
    void redrawChangedTiles();
    void clearChangedTiles();
    void addChangedTile(Tile* tile);

    virtual void draw(SDL_Surface* dst);
    virtual SDL_Rect getRect() const;

private:

    static GridLayer* instance;

    GridLayer();

    std::vector<Tile*> changedTiles;
    std::vector<Tile*> animatedTiles;

};

#endif//GLIDLAYER_HPP
