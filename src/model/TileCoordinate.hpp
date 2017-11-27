#ifndef TILE_COORDINATE_HPP
#define TILE_COORDINATE_HPP

#include <cstdint>
#include "../model/Coordinate.hpp"

class TileCoordinate : public Coordinate {
public:
    static const long SIZE = 25;
    TileCoordinate(long x, long y);
    explicit TileCoordinate(Coordinate coordinate);
    Coordinate toCoordinate() const;
};

#endif
