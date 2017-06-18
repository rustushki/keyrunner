#include "../controller/EditController.hpp"
#include "../controller/Options.hpp"
#include "../model/LevelManager.hpp"
#include "../uitk/Layer.hpp"

/**
 * Constructor.
 * <p>
 * Creates a new level or loads an existing one depending on the initial state of the model.
 * @param model
 * @param display
 */
EditController::EditController(PlayModel *model, Display* display) : BaseController(model, display) {
    // Create New Level for Edit
    if (Options::getCreateNewLevel()) {
        getModel()->setLevelNum((uint16_t) (LevelManager::GetTotal() + 1));
        LevelManager::New(getModel()->getLevelNum());

        // Load Existing Level for Edit
    } else {
        getModel()->setLevelNum(Options::getStartingLevel());
        LevelManager::Read(getModel()->getLevelNum());
    }

}

/**
 * Initialize the game elements for edit mode.
 */
void EditController::gameLoop() {
    // Limit to 25 frames per second
    uint32_t fps = 25;
    uint32_t maxDelay = 1000 / fps;

    while (getModel()->getState() != QUIT) {
        // Each iteration represents a frame

        // Begin preparing the frame
        uint32_t workStart = SDL_GetTicks();

        // Build and present the frame
        getDisplay()->draw();

        processInput();

        // Determine how much time we have left after doing work
        uint32_t workEnd = SDL_GetTicks();
        long workDuration = (long) workEnd - (long) workStart;
        long remainingTime = (long) maxDelay - workDuration;

        // Sleep any remaining time so that we don't hog the CPU
        if (remainingTime > 0) {
            SDL_Delay(remainingTime);
        }
    }
}

/**
 * Handle events for edit mode.
 */
void EditController::processInput() {
    // Wait for an Event.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Key down.
        if (event.type == SDL_KEYDOWN) {
            // User Presses Q
            if (event.key.keysym.sym == SDLK_q) {
                getModel()->setState(QUIT);
                break;

            // Pass all other keys to the parent of the selected layer.  Or the
            // selected layer if it's a root layer.
            } else {
                Layer* selected = Layer::getSelectedLayer();
                if (selected != NULL) {
                    // Find the Event Handling Layer.  It'll either be the
                    // selected layer's parent or the selected layer (depending
                    // on whether the selected layer has no parent).
                    Layer* handler = selected->getParent();
                    if (handler == NULL) {
                        handler = selected;
                    }

                    handler->onKeyDown(event.key.keysym.sym);
                }
            }

            // Handle Quit Event.
        } else if (event.type == SDL_QUIT) {
            getModel()->setState(QUIT);
            break;

        }
    }
}
