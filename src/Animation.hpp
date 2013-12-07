#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include "AnimationType.hpp"
#include "SpriteSheet.hpp"

class Animation {

public:
    static Animation* AnimationFactory(AnimationType at);
    bool advance();
    void blit(SDL_Surface* srf);
    void move(uint16_t x, uint16_t y);
    bool isAnimating() const;
    void play();
    void stop();

    uint16_t getCurrentStill() const;

private:
    Animation(AnimationType at);
    static uint16_t              GetSPS      (AnimationType at);
    static std::string       GetSpriteSFN(AnimationType at);
    static std::vector<uint16_t> GetFrameSize(AnimationType at);
    static std::vector<uint16_t> GetFrameList(AnimationType at);
    SpriteSheet* sheet;
    uint16_t sps;
    bool firstBlit;
    std::vector<uint16_t> frameList;
    uint16_t currentStill;
    AnimationType type;
    uint16_t x;
    uint16_t y;
    bool shouldAdvance;

    uint16_t framesPerStill;
    uint16_t advanceCount;

};

#endif//ANIMATION_HPP
