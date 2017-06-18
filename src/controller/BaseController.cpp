#include <sstream>
#include <SDL2/SDL_ttf.h>

#include "BaseController.hpp"
#include "../view/ConveyorAnimation.hpp"
#include "../view/GridLayer.hpp"
#include "../view/EditRootLayer.hpp"

/**
 * Constructor.
 * <p>
 * Sets the provided fields for subclasses.
 * @param model
 * @param rootLayer
 * @param window
 * @param renderer
 */
BaseController::BaseController(PlayModel *model, RootLayer *rootLayer, SDL_Window *window, SDL_Renderer *renderer) {
    this->model = model;
    this->rootLayer = rootLayer;
    this->window = window;
    this->renderer = renderer;
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
 * Clear the back frame, redraw everything onto it, then present it.
 */
void BaseController::updateDisplay() {
    // Clear the back frame to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);

    // Update and draw the RootLayer (and all nested layers beneath) onto the back frame
    rootLayer->update();
    rootLayer->draw(renderer);

    // Present the back frame
    SDL_RenderPresent(renderer);
}

/**
 * Return the root model that the Controller addresses.
 * @return PlayModel*
 */
PlayModel* BaseController::getModel() {
    return model;
}

/**
 * Return the renderer that the Controller will draw to.
 * @return SDL_Renderer*
 */
SDL_Renderer* BaseController::getRenderer() {
    return renderer;
}

/**
 * Return the window that the Controller's renderer presents to.
 * @return SDL_Window*
 */
SDL_Window* BaseController::getWindow() {
    return window;
}

/**
 * Return the root layer
 * @return RootLayer*
 */
RootLayer* BaseController::getRootLayer() {
    return rootLayer;
}
