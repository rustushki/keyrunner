#ifndef PLAY_MODEL_HPP
#define PLAY_MODEL_HPP

#include <utility>
#include <stdint.h>
#include <vector>

#include "../model/Direction.hpp"
#include "../model/TileType.hpp"
#include "../model/State.hpp"
#include "../model/BaseModel.hpp"

class PlayModel : public BaseModel {

public:
    long getTimeClock();
    void setTimeClock(uint16_t timeClockValue);
    void decrementTimeClock(uint16_t step);
    void incrementTimeClock(uint16_t step);

    // Garbage
    static PlayModel* GetInstance();

private:
    PlayModel();

    long timeClock;

    // Garbage
    static PlayModel* instance;
};

#endif
