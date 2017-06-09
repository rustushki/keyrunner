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

void KeyRunner::play() {
    // Initialize the PlayModel.
    playModel = PlayModel::GetInstance();
    playModel->setTimeClock(50000);

    playModel->setState(PLAY);
    playModel->setLevelNum(Options::getStartingLevel());

    levelLock            = SDL_CreateMutex();
    levelCond            = SDL_CreateCond();
    levelLoadLock        = SDL_CreateMutex();

    if (init()) {
        // There's not a good place for these yet.  Putting them here for now.
        KeyAnimation    = AnimationFactory::Build(ANIMATION_TYPE_KEY);
        PlayerAnimation = AnimationFactory::Build(ANIMATION_TYPE_PUMPKIN);

        SDL_Thread *cyThread = SDL_CreateThread(&convey, "convey",this);

        uint32_t fps = 25;
        uint32_t maxDelay = 1000 / fps;

        LevelManager::Read(playModel->getLevelNum());

        while(playModel->getState() != QUIT) {
            uint32_t workStart = SDL_GetTicks();

            //updateModel();
            updateDisplay();

            if (playModel->isComplete()) {
                playModel->setLevelNum(playModel->getLevelNum() + (uint16_t) 1);
                LevelManager::Read(playModel->getLevelNum());
                playModel->incrementTimeClock(6000);
            }

            processInput();

            uint32_t workEnd = SDL_GetTicks();
            long workDuration = (long) workEnd - (long) workStart;
            long remainingTime = (long) maxDelay - workDuration;

            if (remainingTime > 0) {
                SDL_Delay(remainingTime);
            }

            // Check for winning/losing conditions
            // If the clock runs down to 0; game over
			playModel->decrementTimeClock(SDL_GetTicks() - workStart);
            if (playModel->getTimeClock() <= 0) {
                exitGame();
            }
        }

        SDL_WaitThread(cyThread, NULL);
    } else {
        // TODO: What to do if we fail to initialize?
        // Need a system for handling failures.
    }
}

void KeyRunner::edit() {
    // Initialize the PlayModel.
    playModel = PlayModel::GetInstance();

    playModel->setState(EDIT);

    levelLoadLock        = SDL_CreateMutex();

    if (init()) {
        // There's not a good place for these yet.  Putting them here for now.
        KeyAnimation    = AnimationFactory::Build(ANIMATION_TYPE_KEY);
        PlayerAnimation = AnimationFactory::Build(ANIMATION_TYPE_PUMPKIN);

        SDL_LockMutex(levelLoadLock);

        // Create New Level for Edit
        if (Options::getCreateNewLevel()) {
            playModel->setLevelNum((uint16_t) (LevelManager::GetTotal() + 1));
            LevelManager::New(playModel->getLevelNum());

        // Load Existing Level for Edit
        } else {
            playModel->setLevelNum(Options::getStartingLevel());
            LevelManager::Read(playModel->getLevelNum());
        }

        // Signal that it's OK to observe level tiles now.
        SDL_UnlockMutex(levelLoadLock);

        editHandleEvents();
    } else {
        // TODO: What to do if we fail to initialize?
        // Need a system for handling failures.
    }
}

RootLayer* KeyRunner::getRootLayer() {
    return rootLayer;
}

void KeyRunner::exitGame() {
    // Signal the Event loop to exit.
    SDL_Event quitEvent;
    quitEvent.type = SDL_QUIT;
    SDL_PushEvent(&quitEvent);

    // Let all other threads know that it's time to exit.
    playModel->setState(QUIT);
}

/* ------------------------------------------------------------------------------
 * init - Initialize the screen to 640x480x16.  Initialize the font system.
 * Set the window caption.  Initialize all layer objects.  Upon any failure,
 * return false.
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
 */
void KeyRunner::moveDirection(Direction d) {
    SDL_LockMutex(levelLoadLock);
    GridLayer::GetInstance()->movePlayer(d);
    SDL_UnlockMutex(levelLoadLock);
}


/* ------------------------------------------------------------------------------
 * convey - Thread. Every 100 ms, check to see if the player is on a conveyor
 * tile.  If so, move them to the next conveyor tile on the belt.
 */
int KeyRunner::convey(void* game) {

    KeyRunner* gameInstance = (KeyRunner*) game;

    // Convey only while the game has not yet been quit.
    while(gameInstance->playModel->getState() != QUIT) {

        // Don't attempt to convey if the level is being loaded.
        SDL_LockMutex(gameInstance->levelLoadLock);

        // If the tile in a conveyor tile,
        if (gameInstance->playModel->isConveyor(gameInstance->playModel->getPlayerCoord())) {

            // Convey the player to the next tile.
            TileCoord newTileCoord = gameInstance->playModel->getNextConveyorTileCoord(
                    gameInstance->playModel->getPlayerCoord());
            TileLayer* newTile = GridLayer::GetInstance()->getTile(newTileCoord.first, newTileCoord.second);
            if (GridLayer::GetInstance()->movePlayerToTile(newTile)) {
                if (gameInstance->playModel->isComplete()){
                    SDL_UnlockMutex(gameInstance->levelLock);
                    SDL_CondSignal(gameInstance->levelCond);
                }
            }

        }

        // Indicate that it's OK to load a new level.
        SDL_UnlockMutex(gameInstance->levelLoadLock);

        // Delay 100 ms before conveying the player again..
        SDL_Delay(100);
    }

    return 0;
}

/**
 * Clear the back frame, redraw everything onto it, then present it.
 */
void KeyRunner::updateDisplay() {
    // Do not allow level loading while the back frame is being prepared
    SDL_LockMutex(levelLoadLock);

    // Clear the back frame
    SDL_RenderClear(renderer);

    // Update and draw the RootLayer (and all nested layers beneath).
    rootLayer->update();
    rootLayer->draw(renderer);

    SDL_RenderPresent(renderer);

    // Allow level loading now that the back frame has been presented
    SDL_UnlockMutex(levelLoadLock);
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
                    SDL_UnlockMutex(levelLock);
                    SDL_CondSignal(levelCond);
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
