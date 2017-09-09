#include "../controller/BoardController.hpp"

/**
 * Constructor.
 * <p>
 * Builds the level manager which is used by the subclasses to populate the BoardModel.
 * @param model
 * @param display
 */
BoardController::BoardController(BoardModel* model, Display* display) : DisplayController(model, display) {
    this->levelManager = new LevelManager(model);
}

/**
 * Get the Level Manager which can load levels into the board or save levels to persistent storage from the board.
 * @return LevelManager*
 */
LevelManager* BoardController::getLevelManager() const {
    return levelManager;
}