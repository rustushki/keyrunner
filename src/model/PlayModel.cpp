#include <sstream>
#include "../model/PlayModel.hpp"
#include "../controller/KeyRunner.hpp"

PlayModel* PlayModel::instance = NULL;

PlayModel* PlayModel::GetInstance() {
    if (instance == NULL) {
        instance = new PlayModel();
    }

    return instance;
}

/**
 * Constructor.
 */
PlayModel::PlayModel() : BaseModel() {}

long PlayModel::getTimeClock() {
    return timeClock;
}

void PlayModel::setTimeClock(uint16_t timeClockValue) {
    timeClock = timeClockValue;
}

void PlayModel::decrementTimeClock(uint16_t step) {
    timeClock -= step;
}

void PlayModel::incrementTimeClock(uint16_t step) {
    timeClock += step;
}

