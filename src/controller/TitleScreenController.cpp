#include "TitleScreenController.hpp"

/**
 * Constructor.
 * @param model
 * @param display
 */
TitleScreenController::TitleScreenController(BoardModel *model, Display *display) : BaseController(model, display) {}

/**
 * Destructor.
 */
TitleScreenController::~TitleScreenController() {}

/**
 * Title Screen input.
 */
void TitleScreenController::processInput() {}

/**
 * Update the TitleScreenModel.
 * @param frameDuration
 */
void TitleScreenController::updateModel(long frameDuration) {}

/**
 * Doesn't return true.
 * @return
 */
bool TitleScreenController::checkExitConditions() const {
    return false;
}
