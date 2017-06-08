#ifndef ANIMATION_FACTORY_HPP
#define ANIMATION_FACTORY_HPP

#include <vector>

#include "AnimationType.hpp"
#include "../controller/KeyRunner.hpp"

class Animation;

class AnimationFactory {
    public:
        static Animation* Build(AnimationType animationType);

    private:
        static std::string GetSpriteSFN(AnimationType animationType);
        static std::vector<uint16_t> GetFrameList(AnimationType animationType);
        static std::vector<uint16_t> GetFrameSize(AnimationType animationType);
        static uint16_t GetSPS(AnimationType animationType);
};

#endif
