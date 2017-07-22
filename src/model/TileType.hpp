#ifndef TILE_ENUM_HPP
#define TILE_ENUM_HPP

#include "../view/AnimationType.hpp"

enum class TileType : int {
    Empty = 0,
    Wall = 1,
    Door = 2,
    TeleporterRed = 3,
    TeleporterGreen = 4,
    TeleporterBlue = 5,
    ConveyorUp = 6,
    ConveyorDown = 7,
    ConveyorLeft = 8,
    ConveyorRight = 9,
    Count = 10
};

#endif
