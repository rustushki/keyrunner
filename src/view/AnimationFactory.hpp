#ifndef ANIMATION_FACTORY_HPP
#define ANIMATION_FACTORY_HPP

#include <vector>
#include <map>

#include "../view/AnimationType.hpp"
#include "../controller/KeyRunner.hpp"

class Animation;
class SpriteSheet;

class AnimationFactory {
    public:
        explicit AnimationFactory(SDL_Renderer* renderer);
        ~AnimationFactory();
        Animation* build(AnimationType animationType);

    private:
        SpriteSheet* getSpriteSheet(AnimationType animationType);
        std::string getSpriteSheetFileName(AnimationType animationType);
        std::vector<uint16_t> getFrameList(AnimationType animationType);
        std::vector<uint16_t> getFrameSize(AnimationType animationType);
        uint16_t getStillsPerSecond(AnimationType animationType);

        SDL_Renderer* renderer;
        std::map<AnimationType, SpriteSheet*> spriteSheetMap;
};

#endif
