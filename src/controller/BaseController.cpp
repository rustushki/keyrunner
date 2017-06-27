#include <sstream>
#include <SDL2/SDL_ttf.h>

#include "../controller/BaseController.hpp"

/**
 * Constructor.
 * <p>
 * Sets the provided fields for subclasses.
 * @param model
 * @param display
 */
BaseController::BaseController(BaseModel* model, Display* display) {
    this->model = model;
    this->display = display;
    this->levelManager = new LevelManager(model->getBoard());
}

/**
 * If it has been 100ms since the last time the player was conveyed, check to see if they're on a conveyor tile.  If so,
 * convey them in the direction of the conveyor tile.
 */
void BaseController::conveyPlayer() {
    static uint32_t lastConveyance = 0;
    const int ticksBetweenConveyance = 100;
    BoardModel* board = getModel()->getBoard();

    if (SDL_GetTicks() - lastConveyance >= ticksBetweenConveyance || lastConveyance == 0) {

        // If the player is on a conveyor tile, convey the player to the next tile on the belt
        if (board->isConveyor(board->getPlayerCoord())) {
            TileCoord newTileCoord = board->getNextConveyorTileCoord(board->getPlayerCoord());
            board->setPlayerCoord(newTileCoord);
        }

        lastConveyance = SDL_GetTicks();
    }
}

/**
 * Return the root model that the Controller addresses.
 * @return BaseModel*
 */
BaseModel* BaseController::getModel() const {
    return model;
}

/**
 * Get the display where views are put onto the screen.
 * @return Display*
 */
Display *BaseController::getDisplay() {
    return display;
}

/**
 * Get the Level Manager which can load levels into the board or save levels to persistent storage from the board.
 * @return LevelManager*
 */
LevelManager* BaseController::getLevelManager() const {
    return levelManager;
}

