#include "../model/TileCoordinate.hpp"

/**
 * Constructor.
 * @param x
 * @param y
 */
TileCoordinate::TileCoordinate(uint32_t x, uint32_t y) : Coordinate(x, y) {}

/**
 * Convert the coordinate into a TileCoordinate.
 * @return
 */
TileCoordinate::TileCoordinate(Coordinate coordinate) : Coordinate(
        coordinate.getX() / TileCoordinate::SIZE,
        coordinate.getY() / TileCoordinate::SIZE
) {}

/**
 * Convert the TileCoordinate into a coordinate.
 * <p>
 * This is really just the coordinate of the top left corner of the tile.
 * @return Coordinate
 */
Coordinate TileCoordinate::toCoordinate() const {
    return {getX() * SIZE, getY() * SIZE};
}

