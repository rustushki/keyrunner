#ifndef TILE_COORDINATE_HPP
#define TILE_COORDINATE_HPP

#include <cstdint>
#include "../model/Coordinate.hpp"

class TileCoordinate : public Coordinate {
public:
    static const uint32_t SIZE = 25;
    TileCoordinate(uint32_t x, uint32_t y);
    explicit TileCoordinate(Coordinate coordinate);
    Coordinate toCoordinate() const;
};

#endif
