#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include "../uitk/SpriteSheet.hpp"

class Animation {

public:
    Animation(SpriteSheet* spriteSheet, std::vector<uint16_t> frameList, uint16_t stillsPerSecond);
    bool advance();
    void draw(SDL_Renderer* renderer);
    void move(uint32_t x, uint32_t y);
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
    uint32_t x;
    uint32_t y;
    bool shouldAdvance;

    uint16_t framesPerStill;
    uint16_t advanceCount;

};

#endif
