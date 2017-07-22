#ifndef ANIMATION_TYPE_FACTORY_HPP
#define ANIMATION_TYPE_FACTORY_HPP

#include "../view/AnimationType.hpp"
#include "../model/TileType.hpp"

class AnimationTypeFactory {
public:
    AnimationType build(TileType tileType);

};

#endif
