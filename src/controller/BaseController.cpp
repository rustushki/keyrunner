
#include "../controller/BaseController.hpp"

/**
 * Constructor.
 * <p>
 * Sets the provided fields for subclasses.
 * @param model
 * @param display
 */
BaseController::BaseController(BoardModel* model, Display* display) {
    this->model = model;
    this->display = display;
    this->levelManager = new LevelManager(model);
}

/**
 * Return the root model that the Controller addresses.
 * @return BaseModel*
 */
BoardModel* BaseController::getModel() const {
    return model;
}

/**
 * Get the display where views are put onto the screen.
 * @return Display*
 */
Display *BaseController::getDisplay() const {
    return display;
}

/**
 * Get the Level Manager which can load levels into the board or save levels to persistent storage from the board.
 * @return LevelManager*
 */
LevelManager* BaseController::getLevelManager() const {
    return levelManager;
}

