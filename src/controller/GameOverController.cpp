#include "GameOverController.hpp"

/**
 * Constructor.
 * @param model
 * @param display
 */
GameOverController::GameOverController(GameOverModel *model, Display *display) : DisplayController(model, display) {
    // Do nothing
}

/**
 * Get the GameOverModel for this controller.
 * @return
 */
GameOverModel* GameOverController::getModel() const {
    return dynamic_cast<GameOverModel*>(DisplayController::getModel());
}

/**
 * Update the model for this controller.
 * <p>
 * This is currently a no-op.
 * @param frameDuration
 */
void GameOverController::updateModel(long frameDuration) {
    // Do nothing
}

/**
 * The controller will continue to execute so long as the model is in the LOSE state.
 * @return
 */
bool GameOverController::isStillExecuting() const {
    return getModel()->getState() == LOSE;
}

