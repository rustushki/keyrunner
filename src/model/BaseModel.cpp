#include "BaseModel.hpp"

/**
 * Get the state of the game.
 * @return State
 */
State BaseModel::getState() const {
    return state;
}

/**
 * Set the state of the game.
 * @param State
 */
void BaseModel::setState(State state) {
    this->state = state;
}

