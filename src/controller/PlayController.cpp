#include "../controller/PlayController.hpp"
#include "../controller/Options.hpp"
#include "../model/LevelManager.hpp"
#include "../view/GridLayer.hpp"

/**
 * Constructor.
 * <p>
 * Sets the initial value of the time clock and the starting level based on the provided command line options.
 * @param model
 * @param rootLayer
 * @param window
 * @param renderer
 */
PlayController::PlayController(PlayModel *model, RootLayer* rootLayer, SDL_Window* window, SDL_Renderer* renderer) :
        BaseController(model, rootLayer, window, renderer) {
    // Initialize the PlayModel.
    getModel()->setTimeClock(50000);
    getModel()->setLevelNum(Options::getStartingLevel());
}

/**
 * Initializes the elements required for the 'play' mode.
 *
 * This includes initializing SDL, and kicking off the game loop.
 */
void PlayController::gameLoop() {
    // Limit to 25 frames per second
    uint32_t fps = 25;
    uint32_t maxDelay = 1000 / fps;

    // Read in the starting level
    LevelManager::Read(getModel()->getLevelNum());

    // Begin the game loop and continue while not in the quit state
    while (getModel()->getState() != QUIT) {
        // Each iteration represents a frame

        // Begin preparing the frame
        uint32_t workStart = SDL_GetTicks();

        //updateModel();

        // Build and present the frame
        updateDisplay();

        // Move the player along the conveyor belts (if applicable)
        conveyPlayer();

        // If the level is complete,
        if (getModel()->isComplete()) {
            // Check to see if the next level is beyond the maximum level; i.e. the WIN state
            uint32_t nextLevel = getModel()->getLevelNum() + (uint32_t) + 1;
            if (nextLevel > LevelManager::GetTotal()) {
                getModel()->setState(WIN);
                break;

                // Otherwise, go to next level; adding some extra time to the clock
            } else {
                getModel()->setLevelNum(nextLevel);
                LevelManager::Read(getModel()->getLevelNum());
                getModel()->incrementTimeClock(6000);
            }
        }

        // Handle supported system events
        processInput();

        // Determine how much time we have left after doing work
        uint32_t workEnd = SDL_GetTicks();
        long workDuration = (long) workEnd - (long) workStart;
        long remainingTime = (long) maxDelay - workDuration;

        // Sleep any remaining time so that we don't hog the CPU
        if (remainingTime > 0) {
            SDL_Delay(remainingTime);
        }

        // Check for winning/losing conditions. If the clock runs down to 0; game over
        getModel()->decrementTimeClock(SDL_GetTicks() - workStart);
        if (getModel()->getTimeClock() <= 0) {
            getModel()->setState(LOSE);
            break;
        }
    }
}

/**
 * Process user input for the current frame.
 * <p>
 * Poll for events in a loop.  If the event is a directional key, process it as movement only once for the frame;
 * discarding all other directional key presses.  Also handles Q key to quit, and the SDL_QUIT event type.
 */
void PlayController::processInput() {
    SDL_Event event;
    bool alreadyMoved = false;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            // User Presses Q
            if (event.key.keysym.sym == SDLK_q) {
                getModel()->setState(QUIT);
                break;
            }

            // Limit movement to once per frame
            if (!alreadyMoved) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    moveDirection(DIRECTION_DOWN);

                } else if (event.key.keysym.sym == SDLK_UP) {
                    moveDirection(DIRECTION_UP);

                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    moveDirection(DIRECTION_LEFT);

                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    moveDirection(DIRECTION_RIGHT);

                }
                alreadyMoved = true;

                // If the prior movement causes the level to be complete,
                // signal that the new level may be loaded.
                if (getModel()->isComplete()){
                    return;
                }
            }

            // Handle Quit Event.
        } else if (event.type == SDL_QUIT) {
            getModel()->setState(QUIT);
            break;
        }
    }
}

/**
 * Move the player in the provided direction.
 * @param direction
 */
void PlayController::moveDirection(Direction direction) {
    GridLayer::GetInstance()->movePlayer(direction);
}