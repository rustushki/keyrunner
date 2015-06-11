#ifndef GLIDLAYER_HPP
#define GLIDLAYER_HPP

#include <vector>

#include "AnimationType.hpp"
#include "GridLayer.hpp"
#include "TileLayer.hpp"
#include "../game/TileType.hpp"
#include "../model/Direction.hpp"
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
    virtual void draw(SDL_Surface* dst);
    virtual SDL_Rect getRect() const;
    virtual void update();

    // Some mutators.  These are good candidates to make private later.
    void pushAnimatedTile(TileLayer* tile);
    void refreshTiles();

    // Change Player position.
    bool movePlayer(Direction d);
    bool movePlayerToTile(TileLayer*);

    // Const Accessors which describe the current state of the GridLayer.
    TileLayer* getPlayerTile() const;
    bool hasKey(int x, int y) const;
    bool hasPlayer(int x, int y) const;
    TileLayer* getTile(uint16_t x, uint16_t y) const;

private:

    // GridLayer is singleton.
    static GridLayer* instance;
    GridLayer();

    // Various stateful variables which describe the current state of the
    // GridLayer.
    TileLayer* tile[PlayModel::GRID_HEIGHT][PlayModel::GRID_WIDTH];
    TileLayer* tileHasPlayer;
    TileLayer* tileHasKey;
    Animation* keyAnim;
    Animation* playerAnim;
    std::vector<TileLayer*> changedTiles;
    std::vector<TileLayer*> animatedTiles;

    // Various mutators which can change the state of the GridLayer.
    void init();
    void animateTiles();
    void clearAnimatedTiles();
    void clearChangedTiles();
    void addChangedTile(TileLayer* tile);
    void changeTileType(uint16_t x, uint16_t y, TileType tt);

    // Get the Matching Teleporter Tile.
    TileLayer* getMatchingTeleporterTile(TileLayer*);


    // LevelManager can Read and Write the GridLayer to / from Level files.
    friend class LevelManager;

};

#endif//GLIDLAYER_HPP
