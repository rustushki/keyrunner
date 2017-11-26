#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <cstdint>
#include <utility>
#include "../model/TileCoordinate.hpp"

class Coordinate {
public:
    Coordinate(uint32_t x, uint32_t y);

    uint32_t getX() const;
    uint32_t getY() const;

    void setX(uint32_t x);
    void setY(uint32_t y);

    TileCoordinate toTileCoordinate() const;

    bool operator==(Coordinate coordinate) const;

private:
    uint32_t x;
    uint32_t y;

};

#endif
