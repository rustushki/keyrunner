#include <sstream>
#include <SDL2/SDL_ttf.h>

#include "KeyRunner.hpp"
#include "Options.hpp"
#include "../model/LevelManager.hpp"
#include "../uitk/Animation.hpp"
#include "../view/AnimationFactory.hpp"
#include "../view/ConveyorAnimation.hpp"
#include "../view/GridLayer.hpp"
#include "../view/EditRootLayer.hpp"
#include "../view/PlayRootLayer.hpp"

// Items yet to be absorbed into KeyRunner static class.
Animation* KeyAnimation;
Animation* PlayerAnimation;

/**
 * Initializes the elements required for the 'play' mode.
 *
 * This includes initializing SDL, and kicking off the game loop.
 */
void KeyRunner::play() {
    // Initialize the PlayModel.
    playModel = PlayModel::GetInstance();
    playModel->setTimeClock(50000);
    playModel->setState(PLAY);
    playModel->setLevelNum(Options::getStartingLevel());

    // Initialize SDL
    if (init()) {
        // There's not a good place for these yet.  Putting them here for now.
        KeyAnimation    = AnimationFactory::Build(ANIMATION_TYPE_KEY);
        PlayerAnimation = AnimationFactory::Build(ANIMATION_TYPE_PUMPKIN);

        // Limit to 25 frames per second
        uint32_t fps = 25;
        uint32_t maxDelay = 1000 / fps;

        // Read in the starting level
        LevelManager::Read(playModel->getLevelNum());

        // Begin the game loop and continue while not in the quit state
        while (playModel->getState() != QUIT) {
            // Each iteration represents a frame

            // Begin preparing the frame
            uint32_t workStart = SDL_GetTicks();

            //updateModel();

            // Build and present the frame
            updateDisplay();

            // Move the player along the conveyor belts (if applicable)
            conveyPlayer();

            // If the level is complete,
            if (playModel->isComplete()) {
                // Check to see if the next level is beyond the maximum level; i.e. the WIN state
                uint32_t nextLevel = playModel->getLevelNum() + (uint32_t) + 1;
                if (nextLevel > LevelManager::GetTotal()) {
                    playModel->setState(WIN);
                    break;

                // Otherwise, go to next level; adding some extra time to the clock
                } else {
                    playModel->setLevelNum(nextLevel);
                    LevelManager::Read(playModel->getLevelNum());
                    playModel->incrementTimeClock(6000);
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
			playModel->decrementTimeClock(SDL_GetTicks() - workStart);
            if (playModel->getTimeClock() <= 0) {
                exitGame();
            }
        }

    } else {
        // TODO: What to do if we fail to initialize?
        // Need a system for handling failures.
    }
}

/**
 * Initialize the game elements for edit mode.
 */
void KeyRunner::edit() {
    // Initialize the PlayModel.
    playModel = PlayModel::GetInstance();
    playModel->setState(EDIT);

    // Initialize SDL
    if (init()) {
        // There's not a good place for these yet.  Putting them here for now.
        KeyAnimation    = AnimationFactory::Build(ANIMATION_TYPE_KEY);
        PlayerAnimation = AnimationFactory::Build(ANIMATION_TYPE_PUMPKIN);

        // Create New Level for Edit
        if (Options::getCreateNewLevel()) {
            playModel->setLevelNum((uint16_t) (LevelManager::GetTotal() + 1));
            LevelManager::New(playModel->getLevelNum());

        // Load Existing Level for Edit
        } else {
            playModel->setLevelNum(Options::getStartingLevel());
            LevelManager::Read(playModel->getLevelNum());
        }

        editHandleEvents();
    } else {
        // TODO: What to do if we fail to initialize?
        // Need a system for handling failures.
    }
}

/**
 * Get the root layer of the view.
 * @return root layer
 */
RootLayer* KeyRunner::getRootLayer() {
    return rootLayer;
}

/**
 * Set the play model state to QUIT.  Causes the game loop to exit.
 */
void KeyRunner::exitGame() {
    // Signal the Event loop to exit.
    SDL_Event quitEvent;
    quitEvent.type = SDL_QUIT;
    SDL_PushEvent(&quitEvent);

    // Let all other threads know that it's time to exit.
    playModel->setState(QUIT);
}

/**
 * Initializes much of the game dependencies.
 *
 * <ul>
 * <li>creates a window with dimensions of 640x480x16
 * <li>set the window caption
 * <li>creates the renderer
 * <li>starts the font system
 * <li>creates all layer objects
 * </ul>
 *
 * If something fails, return false.
 */
bool KeyRunner::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Couldn't initialize SDL: "<< SDL_GetError() << std::endl;
        return false;
    }

    if (playModel->getState() == EDIT) {
        rootLayer = EditRootLayer::GetInstance();
    } else {
        rootLayer = PlayRootLayer::GetInstance();
    }

    SDL_Rect rlr = rootLayer->getRect();

    std::stringstream title;
    title << "Key Runner r" << VERSION;
    window = SDL_CreateWindow(title.str().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, rlr.w,
        rlr.h, SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        std::cout << "Couldn't create window: "<< SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
        std::cout << "Couldn't create renderer: "<< SDL_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }

    atexit(SDL_Quit);

    playModel = PlayModel::GetInstance();

    return true;
}

/**
 * Move the player in the provided direction.
 * @param direction
 */
void KeyRunner::moveDirection(Direction direction) {
    GridLayer::GetInstance()->movePlayer(direction);
}


/**
 * If it has been 100ms since the last time the player was conveyed, check to see if they're on a conveyor tile.  If so,
 * convey them in the direction of the conveyor tile.
 */
void KeyRunner::conveyPlayer() {
    static uint32_t lastConveyance = 0;
    const int ticksBetweenConveyance = 100;

    if (SDL_GetTicks() - lastConveyance >= ticksBetweenConveyance || lastConveyance == 0) {

        // If the tile in a conveyor tile,
        if (playModel->isConveyor(playModel->getPlayerCoord())) {
            // Convey the player to the next tile
            TileCoord newTileCoord = playModel->getNextConveyorTileCoord(playModel->getPlayerCoord());
            TileLayer* newTile = GridLayer::GetInstance()->getTile(newTileCoord.first, newTileCoord.second);
            GridLayer::GetInstance()->movePlayerToTile(newTile);
        }

        lastConveyance = SDL_GetTicks();
    }
}

/**
 * Clear the back frame, redraw everything onto it, then present it.
 */
void KeyRunner::updateDisplay() {
    // Clear the back frame
    SDL_RenderClear(renderer);

    // Update and draw the RootLayer (and all nested layers beneath).
    rootLayer->update();
    rootLayer->draw(renderer);

    SDL_RenderPresent(renderer);
}

/**
 * Process user input for the current frame.
 * <p>
 * Poll for events in a loop.  If the event is a directional key, process it as movement only once for the frame;
 * discarding all other directional key presses.  Also handles Q key to quit, and the SDL_QUIT event type.
 */
void KeyRunner::processInput() {
    SDL_Event event;
    bool alreadyMoved = false;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            // User Presses Q
            if (event.key.keysym.sym == SDLK_q) {
                exitGame();
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
                if (playModel->isComplete()){
                    return;
                }
            }

        // Handle Quit Event.
        } else if (event.type == SDL_QUIT) {
            exitGame();
            break;
        }
    }
}

/**
 * Handle events for edit mode.
 */
void KeyRunner::editHandleEvents() {
    // Wait for an Event.
    SDL_Event event;
    while (playModel->getState() != QUIT) {
        SDL_WaitEvent(&event);

        // Key down.
        if (event.type == SDL_KEYDOWN) {

            // User Presses Q
            if (event.key.keysym.sym == SDLK_q) {
                exitGame();
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

        } else if (event.type == SDL_KEYUP) {

            // Handle Quit Event.
        } else if (event.type == SDL_QUIT) {
            exitGame();
            break;

        }
    }
}
