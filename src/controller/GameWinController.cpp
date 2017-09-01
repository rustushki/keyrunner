#include "../controller/GameWinController.hpp"

/**
 * Constructor.
 * @param model
 * @param display
 */
GameWinController::GameWinController(BaseModel *model, Display *display) : DisplayController(model, display) {}

/**
 * Get the model for this controller.
 * @return GameWinModel*
 */
GameWinModel *GameWinController::getModel() const {
    return dynamic_cast<GameWinModel*>(DisplayController::getModel());
}

/**
 * Update the model for this controller.
 * @param frameDuration
 */
void GameWinController::updateModel(long frameDuration) {
    // Do nothing
}

/**
 * Returns true if the model state is still WIN.
 * @return bool
 */
bool GameWinController::isStillExecuting() const {
    return getModel()->getState() == WIN;
}
