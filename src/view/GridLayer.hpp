#ifndef GRID_LAYER_HPP
#define GRID_LAYER_HPP

#include <vector>

#include "AnimationType.hpp"
#include "GridLayer.hpp"
#include "TileLayer.hpp"
#include "../model/Direction.hpp"
#include "../model/TileType.hpp"
#include "../model/PlayModel.hpp"
#include "../uitk/Layer.hpp"


class Animation;
class TileLayer;

class GridLayer : public Layer {

public:
    // GridLayer is Singleton.
    static GridLayer* GetInstance();
    ~GridLayer();

    // Required to derive from Layer.
    virtual void draw(SDL_Renderer* renderer);
    virtual SDL_Rect getRect() const;
    virtual void update();

    // Some state changing methods.  These are good candidates to make private later.
    void pushAnimatedTile(TileLayer* tile);

    // Change Player position.
    bool movePlayer(Direction d);
    bool movePlayerToTile(TileLayer*);

    TileLayer* getTile(uint16_t x, uint16_t y) const;

private:

    // GridLayer is singleton.
    static GridLayer* instance;
    GridLayer();

    // Various stateful variables which describe the current state of the
    // GridLayer.
    TileLayer* tile[25][16];
    std::vector<TileLayer*> animatedTiles;

    // Various methods which can change the state of the GridLayer.
    void init();
    void animateTiles();
    void clearAnimatedTiles();
    void changeTileType(uint16_t x, uint16_t y, TileType tt);

    // LevelManager can Read and Write the GridLayer to / from Level files.
    friend class LevelManager;
};

#endif
