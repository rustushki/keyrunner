#ifndef ANIMATION_FACTORY_HPP
#define ANIMATION_FACTORY_HPP

#include <vector>

#include "AnimationType.hpp"
#include "../controller/KeyRunner.hpp"

class Animation;

class AnimationFactory {
    public:
        AnimationFactory(SDL_Renderer* renderer);
        Animation* build(AnimationType animationType);

    private:
        std::string getSpriteSFN(AnimationType animationType);
        std::vector<uint16_t> getFrameList(AnimationType animationType);
        std::vector<uint16_t> getFrameSize(AnimationType animationType);
        uint16_t getSPS(AnimationType animationType);

        SDL_Renderer* renderer;
};

#endif
