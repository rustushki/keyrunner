#include "../model/BaseModel.hpp"

/**
 * Constructor.
 * <p>
 * Creates the board which is common to subclasses.
 */
BaseModel::BaseModel() {
    this->board = new Board();
}

/**
 * Destructor.
 * <p>
 * Destroys the board.
 */
BaseModel::~BaseModel() {
    delete this->board;
}

/**
 * Fetch the board.
 * @return the board
 */
Board* BaseModel::getBoard() {
    return board;
}

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
