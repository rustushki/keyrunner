#include "../model/Coordinate.hpp"

/**
 * Constructor.
 * @param x
 * @param y
 */
Coordinate::Coordinate(long x, long y) {
    this->x = x;
    this->y = y;
}

/**
 * Get the x dimension of the coordinate.
 * @return long
 */
long Coordinate::getX() const {
    return x;
}

/**
 * Get the y dimension of the coordinate.
 * @return long
 */
long Coordinate::getY() const {
    return y;
}

/**
 * Check to see if two coordinates are the same.
 * <p>
 * They will be if the x and y dimensions from each coordinate match.
 * @param coordinate
 * @return
 */
bool Coordinate::operator==(Coordinate coordinate) const {
    return coordinate.x == this->x && coordinate.y == this->y;
}

/**
 * Set the x dimension of the coordinate
 * @param x
 */
void Coordinate::setX(long x) {
    this->x = x;
}

/**
 * Set the y dimension of the coordinate
 * @param y
 */
void Coordinate::setY(long y) {
    this->y = y;
}

/**
 * Is this tile coordinate less than another?
 * <p>
 * This tricky code is roughly equivalent to std::pair's operator<. It is required so that TileCoordinate can be a key
 * in an std::map.
 * @param rhs
 * @return bool
 */
bool Coordinate::operator<(const Coordinate& rhs) const {
    if (getX() < rhs.getX()) {
        return true;
    } else if (rhs.getX() < getX()) {
        return false;
    } else {
        return getY() < rhs.getY();
    }
}
