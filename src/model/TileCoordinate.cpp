#include "../model/Coordinate.hpp"

/**
 * Constructor.
 * @param x
 * @param y
 */
TileCoordinate::TileCoordinate(uint32_t x, uint32_t y) {
    this->x = x;
    this->y = y;
}

/**
 * Get the x dimension of the TileCoordinate.
 * @return uint32_t
 */
uint32_t TileCoordinate::getX() const {
    return x;
}

/**
 * Get the y dimension of the TileCoordinate.
 * @return uint32_t
 */
uint32_t TileCoordinate::getY() const {
    return y;
}

/**
 * Convert the TileCoordinate into a coordinate.
 * <p>
 * This is really just the coordinate of the top left corner of the tile.
 * @return Coordinate
 */
Coordinate TileCoordinate::toCoordinate() const {
    return {x * SIZE, y * SIZE};
}

/**
 * Set the x dimension of the TileCoordinate.
 * @param x
 */
void TileCoordinate::setX(uint32_t x) {
    this->x = x;
}

/**
 * Set the y dimension of the TileCoordinate.
 * @param y
 */
void TileCoordinate::setY(uint32_t y) {
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
bool TileCoordinate::operator<(const TileCoordinate& rhs) const {
    if (getX() < rhs.getX()) {
        return true;
    } else if (rhs.getX() < getX()) {
        return false;
    } else {
        return getY() < rhs.getY();
    }
}

