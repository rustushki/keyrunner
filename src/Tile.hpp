#ifndef TILE_HPP
#define TILE_HPP

#include <vector>
#include "Animation.hpp"
#include "AnimationType.hpp"
#include "Direction.hpp"
#include "TileType.hpp"

class Level;

class Tile : public Layer {

public:
    Tile(TileType type, uint16_t x, uint16_t y, Level*);
    ~Tile();

    static const uint8_t SIZE = 25;

    SDL_Rect getRect() const;

    void draw(SDL_Surface* dst);

    Animation* getAnimation() const;
    static AnimationType TileTypeToAnimType(TileType tt);

    TileType getType() const;

    uint16_t getX() const;
    uint16_t getY() const;

    bool isTeleporter() const;
    bool isDoor() const;
    bool isWall() const;
    bool isConveyor() const;
    bool hasPlayer() const;
    bool hasKey() const;

    Tile* getNextConveyorTile() const;

    Tile* up() const;
    Tile* down() const;
    Tile* left() const;
    Tile* right() const;

    Tile* getTileInDirection(Direction d) const;

    Direction getConveyorDirection() const;

private:
    TileType type;
    Animation* anim;
    Level* level;

    uint16_t x;
    uint16_t y;

};

#endif//TILE_HPP
