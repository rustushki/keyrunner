#ifndef TILE_HPP
#define TILE_HPP

#include <vector>
#include "AnimationType.hpp"
#include "../game/TileType.hpp"
#include "../model/Direction.hpp"
#include "../uitk/Animation.hpp"
#include "../uitk/Layer.hpp"

class Level;

class TileLayer : public Layer {

public:
    TileLayer(TileType type, uint16_t x, uint16_t y);
    ~TileLayer();

    static const uint8_t SIZE = 25;

    SDL_Rect getRect() const;

    void draw(SDL_Surface* dst);

    Animation* getAnimation() const;
    static AnimationType TileTypeToAnimType(TileType tt);

    TileType getType() const;
    void setType(TileType tt);

    uint16_t getX() const;
    uint16_t getY() const;

    bool isTeleporter() const;
    bool isDoor() const;
    bool isWall() const;
    bool isConveyor() const;
    bool hasPlayer() const;
    bool hasKey() const;

    TileLayer* getNextConveyorTile() const;

    TileLayer* up() const;
    TileLayer* down() const;
    TileLayer* left() const;
    TileLayer* right() const;

    TileLayer* getTileInDirection(Direction d) const;

    Direction getConveyorDirection() const;

private:
    TileType type;
    Animation* anim;
    Level* level;

    uint16_t x;
    uint16_t y;

};

#endif//TILE_HPP
