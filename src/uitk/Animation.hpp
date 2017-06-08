#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include "SpriteSheet.hpp"

class Animation {

public:
    Animation(SpriteSheet* spriteSheet, std::vector<uint16_t> frameList, uint16_t stillsPerSecond);
    bool advance();
    void blit(SDL_Renderer* renderer, SDL_Texture* destination);
    void move(uint16_t x, uint16_t y);
    bool isAnimating() const;
    void play();
    void stop();
    uint16_t getWidth() const;
    uint16_t getHeight() const;

    uint16_t getCurrentStill() const;

private:
    SpriteSheet* sheet;
    uint16_t sps;
    bool firstBlit;
    std::vector<uint16_t> frameList;
    uint16_t currentStill;
    uint16_t x;
    uint16_t y;
    bool shouldAdvance;

    uint16_t framesPerStill;
    uint16_t advanceCount;

};

#endif//ANIMATION_HPP
