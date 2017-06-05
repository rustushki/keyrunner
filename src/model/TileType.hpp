#ifndef TILE_TYPE_HPP
#define TILE_TYPE_HPP

#include "../view/AnimationType.hpp"

class TileType {
public:
    TileType();

    constexpr explicit TileType(int id) : id{id} {};
    static constexpr int length() { return 10; }
    constexpr operator int() const { return id; }

    AnimationType toAnimationType();

private:
    int id;
};

constexpr TileType TILE_TYPE_EMPTY(0);
constexpr TileType TILE_TYPE_WALL(1);
constexpr TileType TILE_TYPE_DOOR(2);
constexpr TileType TILE_TYPE_TELEPORTER_RED(3);
constexpr TileType TILE_TYPE_TELEPORTER_GREEN(4);
constexpr TileType TILE_TYPE_TELEPORTER_BLUE(5);
constexpr TileType TILE_TYPE_CONVEY_UP(6);
constexpr TileType TILE_TYPE_CONVEY_DOWN(7);
constexpr TileType TILE_TYPE_CONVEY_LEFT(8);
constexpr TileType TILE_TYPE_CONVEY_RIGHT(9);

#endif//TILE_TYPE_HPP
