#ifndef PLAY_MODEL_HPP
#define PLAY_MODEL_HPP

#include "../model/State.hpp"
#include "../model/BaseModel.hpp"

class PlayModel : public BaseModel {

public:
    PlayModel();

    long getTimeClock();
    void setTimeClock(long timeClockValue);
    void decrementTimeClock(long step);
    void incrementTimeClock(long step);

private:
    long timeClock;
};

#endif
