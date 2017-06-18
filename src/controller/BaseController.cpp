#include <sstream>
#include <SDL2/SDL_ttf.h>

#include "BaseController.hpp"
#include "../view/ConveyorAnimation.hpp"
#include "../view/GridLayer.hpp"

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

        // If the tile in a conveyor tile,
        if (model->isConveyor(model->getPlayerCoord())) {
            // Convey the player to the next tile
            TileCoord newTileCoord = model->getNextConveyorTileCoord(model->getPlayerCoord());
            TileLayer* newTile = GridLayer::GetInstance()->getTile(newTileCoord.first, newTileCoord.second);
            GridLayer::GetInstance()->movePlayerToTile(newTile);
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

