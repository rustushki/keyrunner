#include "../model/PlayModel.hpp"

/**
 * Constructor.
 */
PlayModel::PlayModel() : BaseModel() {}

/**
 * Get the time remaining on the time clock.
 * @return long
 */
long PlayModel::getTimeClock() {
    return timeClock;
}

/**
 * Set the time remaining on the time clock.
 * @param uint16_t
 */
void PlayModel::setTimeClock(long timeClockValue) {
    timeClock = timeClockValue;
}

/**
 * Decrement the time clock by a certain amount.
 * @param step
 */
void PlayModel::decrementTimeClock(long step) {
    timeClock -= step;
}

/**
 * Increment the time clock by a certain amount.
 * @param step
 */
void PlayModel::incrementTimeClock(long step) {
    timeClock += step;
}

