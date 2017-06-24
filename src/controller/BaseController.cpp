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
BaseController::BaseController(PlayModel *model, Display* display) {
    this->model = model;
    this->display = display;
}

/**
 * If it has been 100ms since the last time the player was conveyed, check to see if they're on a conveyor tile.  If so,
 * convey them in the direction of the conveyor tile.
 */
void BaseController::conveyPlayer() {
    static uint32_t lastConveyance = 0;
    const int ticksBetweenConveyance = 100;

    if (SDL_GetTicks() - lastConveyance >= ticksBetweenConveyance || lastConveyance == 0) {

        // If the player is on a conveyor tile, convey the player to the next tile on the belt
        if (getModel()->isConveyor(getModel()->getPlayerCoord())) {
            TileCoord newTileCoord = getModel()->getNextConveyorTileCoord(getModel()->getPlayerCoord());
            getModel()->setPlayerCoord(newTileCoord);
        }

        lastConveyance = SDL_GetTicks();
    }
}

/**
 * Return the root model that the Controller addresses.
 * @return PlayModel*
 */
PlayModel* BaseController::getModel() {
    return model;
}

Display *BaseController::getDisplay() {
    return display;
}

