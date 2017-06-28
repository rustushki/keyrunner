#ifndef PLAY_MODEL_HPP
#define PLAY_MODEL_HPP

#include <stdint.h>

#include "../model/State.hpp"
#include "../model/BaseModel.hpp"

class PlayModel : public BaseModel {

public:
    long getTimeClock();
    void setTimeClock(long timeClockValue);
    void decrementTimeClock(long step);
    void incrementTimeClock(long step);

    // Garbage
    static PlayModel* GetInstance();

private:
    PlayModel();

    long timeClock;

    // Garbage
    static PlayModel* instance;
};

#endif
