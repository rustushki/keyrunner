#include <iostream>

#include "TileType.hpp"
#include "../controller/KeyRunner.hpp"

TileType::TileType() {}

AnimationType TileType::toAnimationType() {
    switch (*this) {
        case TILE_TYPE_EMPTY:
            return ANIMATION_TYPE_EMPTY;

        case TILE_TYPE_WALL:
            return ANIMATION_TYPE_WALL;

        case TILE_TYPE_DOOR:
            return ANIMATION_TYPE_DOOR;

        case TILE_TYPE_TELEPORTER_RED:
            return ANIMATION_TYPE_TELEPORTER_RED;

        case TILE_TYPE_TELEPORTER_GREEN:
            return ANIMATION_TYPE_TELEPORTER_GREEN;

        case TILE_TYPE_TELEPORTER_BLUE:
            return ANIMATION_TYPE_TELEPORTER_BLUE;

        case TILE_TYPE_CONVEY_UP:
            return ANIMATION_TYPE_CONVEY_UP;

        case TILE_TYPE_CONVEY_DOWN:
            return ANIMATION_TYPE_CONVEY_DOWN;

        case TILE_TYPE_CONVEY_LEFT:
            return ANIMATION_TYPE_CONVEY_LEFT;

        case TILE_TYPE_CONVEY_RIGHT:
            return ANIMATION_TYPE_CONVEY_RIGHT;

        default:
            std::cout << "Could not determine AnimationType." << std::endl;
            std::cout << "Invalid TileType." << std::endl;
            keyRunner.exitGame();
            break;
    }

    // Should not arrive here.
    std::cout << "Logic error detected." << std::endl;
    keyRunner.exitGame();

    // Should certainly never arrive here.
    return ANIMATION_TYPE_EMPTY;
}
