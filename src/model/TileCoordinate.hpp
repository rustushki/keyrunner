#ifndef TILE_COORDINATE_HPP
#define TILE_COORDINATE_HPP

#include <cstdint>
class Coordinate;

class TileCoordinate {
public:
    TileCoordinate(uint32_t x, uint32_t y);

    uint32_t getX() const;
    uint32_t getY() const;

    void setX(uint32_t x);
    void setY(uint32_t y);

    Coordinate toCoordinate() const;

    static const uint32_t SIZE = 25;

    bool operator<(const TileCoordinate& rhs) const;

private:
    uint32_t x;
    uint32_t y;

};

#endif
