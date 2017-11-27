#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <cstdint>
#include <utility>

class TileCoordinate;

class Coordinate {
public:
    Coordinate(uint32_t x, uint32_t y);

    uint32_t getX() const;
    uint32_t getY() const;

    void setX(uint32_t x);
    void setY(uint32_t y);

    bool operator==(Coordinate coordinate) const;
    bool operator<(const Coordinate& rhs) const;

private:
    uint32_t x;
    uint32_t y;

};

#endif
