#include <sstream>
#include "../model/AnimationTypeFactory.hpp"

/**
 * Builds an AnimationType from a TileType.
 * @return AnimationType
 */
AnimationType AnimationTypeFactory::build(TileType tileType) {
    switch (tileType) {
        case TileType::Empty:
            return ANIMATION_TYPE_EMPTY;

        case TileType::Wall:
            return ANIMATION_TYPE_WALL;

        case TileType::Door:
            return ANIMATION_TYPE_DOOR;

        case TileType::TeleporterRed:
            return ANIMATION_TYPE_TELEPORTER_RED;

        case TileType::TeleporterGreen:
            return ANIMATION_TYPE_TELEPORTER_GREEN;

        case TileType::TeleporterBlue:
            return ANIMATION_TYPE_TELEPORTER_BLUE;

        case TileType::ConveyorUp:
            return ANIMATION_TYPE_CONVEY_UP;

        case TileType::ConveyorDown:
            return ANIMATION_TYPE_CONVEY_DOWN;

        case TileType::ConveyorLeft:
            return ANIMATION_TYPE_CONVEY_LEFT;

        case TileType::ConveyorRight:
            return ANIMATION_TYPE_CONVEY_RIGHT;

        default:
            std::stringstream errorMessage;
            errorMessage << "Could not determine AnimationType because of invalid TileType";
            throw std::invalid_argument(errorMessage.str());
    }
}
