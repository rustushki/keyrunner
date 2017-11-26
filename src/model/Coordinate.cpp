#include "../model/Coordinate.hpp"

/**
 * Constructor.
 * @param x
 * @param y
 */
Coordinate::Coordinate(uint32_t x, uint32_t y) {
    this->x = x;
    this->y = y;
}

/**
 * Get the x dimension of the coordinate.
 * @return uint32_t
 */
uint32_t Coordinate::getX() const {
    return x;
}

/**
 * Get the y dimension of the coordinate.
 * @return uint32_t
 */
uint32_t Coordinate::getY() const {
    return y;
}

/**
 * Convert the coordinate into a TileCoordinate.
 * @return
 */
TileCoordinate Coordinate::toTileCoordinate() const {
    return {x / TileCoordinate::SIZE, y / TileCoordinate::SIZE};
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
void Coordinate::setX(uint32_t x) {
    this->x = x;
}

/**
 * Set the y dimension of the coordinate
 * @param y
 */
void Coordinate::setY(uint32_t y) {
    this->y = y;
}
