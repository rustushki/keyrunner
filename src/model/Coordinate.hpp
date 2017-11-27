#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <cstdint>
#include <utility>

class TileCoordinate;

class Coordinate {
public:
    Coordinate(long x, long y);

    long getX() const;
    long getY() const;

    void setX(long x);
    void setY(long y);

    bool operator==(Coordinate coordinate) const;
    bool operator<(const Coordinate& rhs) const;

private:
    long x;
    long y;

};

#endif
