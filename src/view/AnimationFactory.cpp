#include <sstream>
#include "AnimationFactory.hpp"
#include "../uitk/Animation.hpp"

#define FL(x,y) frameList.push_back(x);frameList.push_back(y);

/**
 * Build an Animation from an AnimationType.
 * <p>
 * Add more Animations by modifying each of the private methods of this class.
 * @param animationType
 */
Animation* AnimationFactory::Build(AnimationType animationType) {
    try {
        std::string filename = GetSpriteSFN(animationType);
        std::vector<uint16_t> frameList = GetFrameList(animationType);
        std::vector<uint16_t> frameSize = GetFrameSize(animationType);
        uint16_t stillsPerSecond = GetSPS(animationType);

        return new Animation(new SpriteSheet(filename, frameSize[0], frameSize[1]), frameList, stillsPerSecond);

    } catch (std::invalid_argument exception) {
        std::stringstream errorMessage;
        errorMessage << "Couldn't build Animation (type = " << animationType << ") reason: " << exception.what();
        throw std::logic_error(errorMessage.str());
    }
}

/**
 * Given an animation type, determine the path to the sprite sheet associated with that animation.
 * @param animationType
 */
std::string AnimationFactory::GetSpriteSFN(AnimationType animationType) {

    std::string fn;

    switch (animationType) {

        case ANIMATION_TYPE_EMPTY:
            fn = "tile.png";
            break;

        case ANIMATION_TYPE_WALL:
            fn = "wall.png";
            break;

        case ANIMATION_TYPE_DOOR:
            fn = "door.png";
            break;

        case ANIMATION_TYPE_TELEPORTER_RED:
            fn = "teleporter_red.png";
            break;

        case ANIMATION_TYPE_TELEPORTER_GREEN:
            fn = "teleporter_green.png";
            break;

        case ANIMATION_TYPE_TELEPORTER_BLUE:
            fn = "teleporter_blue.png";
            break;

        case ANIMATION_TYPE_CONVEY_DOWN:
        case ANIMATION_TYPE_CONVEY_UP:
        case ANIMATION_TYPE_CONVEY_LEFT:
        case ANIMATION_TYPE_CONVEY_RIGHT:
            fn = "conveyor.png";
            break;

        case ANIMATION_TYPE_KEY:
            fn = "key.png";
            break;

        case ANIMATION_TYPE_PUMPKIN:
            fn = "pumpkin.png";
            break;

        default:
            std::stringstream errorMessage;
            errorMessage << "Couldn't determine sprite sheet due to invalid AnimationType";
            throw std::invalid_argument(errorMessage.str());
    }


    // Prepend the image path.
    std::stringstream ss;
    ss << IMAGE_PATH << fn;

    // Return path to the sprite sheet file.
    return ss.str();
}

/**
 * Given an AnimationType, return a vector of unsigned ints which denote the list of frames within a SpriteSheet for
 * that kind of animation.
 * @param animationType
 */
std::vector<uint16_t> AnimationFactory::GetFrameList(AnimationType animationType) {
    std::vector<uint16_t> frameList;

    switch (animationType) {

        case ANIMATION_TYPE_CONVEY_DOWN:
            FL(1,0)FL(1,1)FL(1,2)FL(1,3)
                FL(1,4)FL(1,5)FL(1,6)FL(1,7)
                FL(1,8)FL(1,9)FL(1,10)FL(1,11)
                break;

        case ANIMATION_TYPE_CONVEY_UP:
            FL(2,0)FL(2,1)FL(2,2)FL(2,3)
                FL(2,4)FL(2,5)FL(2,6)FL(2,7)
                FL(2,8)FL(2,9)FL(2,10)FL(2,11)
                break;

        case ANIMATION_TYPE_CONVEY_LEFT:
            FL(0,0)FL(0,1)FL(0,2)FL(0,3)
                FL(0,4)FL(0,5)FL(0,6)FL(0,7)
                FL(0,8)FL(0,9)FL(0,10)FL(0,11)
                break;

        case ANIMATION_TYPE_CONVEY_RIGHT:
            FL(3,0)FL(3,1)FL(3,2)FL(3,3)
                FL(3,4)FL(3,5)FL(3,6)FL(3,7)
                FL(3,8)FL(3,9)FL(3,10)FL(3,11)
                break;

        case ANIMATION_TYPE_EMPTY:
        case ANIMATION_TYPE_WALL:
        case ANIMATION_TYPE_DOOR:
        case ANIMATION_TYPE_TELEPORTER_RED:
        case ANIMATION_TYPE_TELEPORTER_GREEN:
        case ANIMATION_TYPE_TELEPORTER_BLUE:
        case ANIMATION_TYPE_KEY:
        case ANIMATION_TYPE_PUMPKIN:
            FL(0,0);
            break;

        default:
            std::stringstream errorMessage;
            errorMessage << "Couldn't build frame list due to invalid AnimationType";
            throw std::invalid_argument(errorMessage.str());

    }

    return frameList;
}

/**
 * Given an AnimationType, return the frame size of the SpriteSheet associated with that Animation.
 * @param animationType
 */
std::vector<uint16_t> AnimationFactory::GetFrameSize(AnimationType animationType) {
    std::vector<uint16_t> frameSize;

    switch (animationType) {

        case ANIMATION_TYPE_EMPTY:
        case ANIMATION_TYPE_WALL:
        case ANIMATION_TYPE_DOOR:
        case ANIMATION_TYPE_TELEPORTER_RED:
        case ANIMATION_TYPE_TELEPORTER_GREEN:
        case ANIMATION_TYPE_TELEPORTER_BLUE:
        case ANIMATION_TYPE_CONVEY_UP:
        case ANIMATION_TYPE_CONVEY_DOWN:
        case ANIMATION_TYPE_CONVEY_LEFT:
        case ANIMATION_TYPE_CONVEY_RIGHT:
        case ANIMATION_TYPE_KEY:
        case ANIMATION_TYPE_PUMPKIN:
            frameSize.push_back(25);
            frameSize.push_back(25);
            break;

        default:
            std::stringstream errorMessage;
            errorMessage << "Couldn't determine frame size due to invalid AnimationType";
            throw std::invalid_argument(errorMessage.str());
    }

    return frameSize;
}

/**
 * Given an animation type, return the stills per second count.  This may go away in the future as objects are
 * introduced for on screen elements. Such objects might need to adjust and regulate SPS.
 * @param animationType
 */
uint16_t AnimationFactory::GetSPS(AnimationType animationType) {
    uint16_t sps = 0;

    switch (animationType) {

        case ANIMATION_TYPE_EMPTY:
        case ANIMATION_TYPE_WALL:
        case ANIMATION_TYPE_DOOR:
        case ANIMATION_TYPE_TELEPORTER_RED:
        case ANIMATION_TYPE_TELEPORTER_GREEN:
        case ANIMATION_TYPE_TELEPORTER_BLUE:
        case ANIMATION_TYPE_KEY:
        case ANIMATION_TYPE_PUMPKIN:
            sps = 0;
            break;

        case ANIMATION_TYPE_CONVEY_DOWN:
        case ANIMATION_TYPE_CONVEY_UP:
        case ANIMATION_TYPE_CONVEY_LEFT:
        case ANIMATION_TYPE_CONVEY_RIGHT:
            sps = 14;
            break;

        default:
            std::stringstream errorMessage;
            errorMessage << "Couldn't determine stills per second due to invalid AnimationType";
            throw std::invalid_argument(errorMessage.str());
    }

    return sps;
}
