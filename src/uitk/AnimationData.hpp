#ifndef KEYRUNNER_ANIMATION_DATA_HPP
#define KEYRUNNER_ANIMATION_DATA_HPP

#include <vector>
#include "SpriteSheet.hpp"

class AnimationData {
public:
    AnimationData(SpriteSheet* spriteSheet, std::vector<uint16_t> frameList, uint16_t stillsPerSecond);
    bool advance();
    void move(uint16_t x, uint16_t y);
    bool isAnimating() const;
    void play();
    void stop();
    uint16_t getWidth() const;
    uint16_t getHeight() const;

private:
    SpriteSheet* sheet;
    uint16_t sps;
    std::vector<uint16_t> frameList;
    uint16_t currentStill;
    bool shouldAdvance;
    uint16_t x;
    uint16_t y;

    uint16_t framesPerStill;
    uint16_t advanceCount;
};

#endif
