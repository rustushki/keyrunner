#include "../controller/OptionController.hpp"

/**
 * Constructor.
 */
OptionModel::OptionModel() {
    this->startingLevel = 1;
    this->initialState = TITLE;
    this->createNewLevel = false;
}

/**
 * Fetch the starting level provided from the command line (default = 1).
 * @return starting level
 */
uint8_t OptionModel::getStartingLevel() {
    return startingLevel;
}

/**
 * Fetch the state the game should start in (default = PLAY).
 * @return PLAY or EDIT
 */
State OptionModel::getInitialState() {
    return initialState;
}

/**
 * Did the user indicate that they want to create a new level?
 * @return did they?
 */
bool OptionModel::getCreateNewLevel() {
    return createNewLevel;
}

/**
 * Set the starting level.
 * @param startingLevel
 */
void OptionModel::setStartingLevel(uint8_t startingLevel) {
    this->startingLevel = startingLevel;
}

/**
 * Set the initial state of the game.
 * @param initialState
 */
void OptionModel::setInitialState(State initialState) {
    this->initialState = initialState;
}

/**
 * Set whether or not the editor should create a new level.
 * @param createNewLevel
 */
void OptionModel::setCreateNewLevel(bool createNewLevel) {
    this->createNewLevel = createNewLevel;
}

/**
 * Get the state of the model.
 * <p>
 * Always returns OPTION
 * @return State
 */
State OptionModel::getState() const {
    return OPTION;
}

/**
 * No-op.
 * @param newState
 */
void OptionModel::setState(State newState) {
    // Do nothing
}
