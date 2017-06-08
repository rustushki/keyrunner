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
    initialLevelLoadLock = SDL_CreateMutex();
    initialLevelLoadCond = SDL_CreateCond();

    if (init()) {
        // There's not a good place for these yet.  Putting them here for now.
        KeyAnimation    = AnimationFactory::Build(ANIMATION_TYPE_KEY);
        PlayerAnimation = AnimationFactory::Build(ANIMATION_TYPE_PUMPKIN);

        SDL_Thread *ulThread = SDL_CreateThread(&updateLevel, "updateLevel", this);

        SDL_LockMutex(initialLevelLoadLock);
        SDL_CondWait(initialLevelLoadCond, initialLevelLoadLock);

        SDL_Thread *ctThread = SDL_CreateThread(&clockTick, "clockTick",this);
        SDL_Thread *cyThread = SDL_CreateThread(&convey, "convey",this);
        SDL_Thread *phThread = SDL_CreateThread(&playHandleEvents, "playHandleEvents", this);

        while(playModel->getState() != QUIT) {
            uint32_t fps = 25;
            uint32_t delay = 1000 / fps;

            updateDisplay();
            SDL_Delay(delay);
        }

        SDL_WaitThread(phThread, NULL);
        SDL_WaitThread(cyThread, NULL);
        SDL_WaitThread(ctThread, NULL);
        SDL_WaitThread(ulThread, NULL);
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
    initialLevelLoadLock = SDL_CreateMutex();
    initialLevelLoadCond = SDL_CreateCond();

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
    // Unblock the updateLevel thread so that it can free the Level's memory.
    SDL_UnlockMutex(levelLock);
    SDL_CondSignal(levelCond);

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

int KeyRunner::clockTick(void* game) {
    KeyRunner* gameInstance = (KeyRunner*) game;

    while (gameInstance->playModel->getTimeClock() > 0 && gameInstance->playModel->getState() == PLAY) {
        uint16_t step = 100;
        SDL_Delay(step);
        gameInstance->playModel->decrementTimeClock(step);
    }

    SDL_CondSignal(gameInstance->levelCond);
    gameInstance->exitGame();
    return 0;
}

/* ------------------------------------------------------------------------------
 * moveDirection - Move the player in the provided direction and pay close
 * attention to keyboard input while doing so.  This must run in the same
 * thread that initialized SDL.
 *
 */
void KeyRunner::moveDirection(Direction d) {

    const Uint8* keyState = SDL_GetKeyboardState(nullptr);

    // How long to wait in MS before assuming the user wishes to 'auto move'
    uint16_t holdDelayMs = 200;

    // How frequently to check to see if the user is holding down an arrow button.
    uint16_t holdDelayPollCheck = 10;

    // Count of poll attempts to check to see if the user is holding an arrow.
    uint16_t holdDelayPollTries = holdDelayMs/holdDelayPollCheck;

    // Number of atomic moves per second.  Each 'move' causes the player to
    // traverse one tile.
    uint16_t movesPerSecond = 10;

    // The amount of delay between tile moves.
    uint16_t autoMoveDelay = (uint16_t) (1000/movesPerSecond);

    // Based on the provided direction, infer which key the player must be
    // pressing.
    int sdlKey = 0;
    if (d == DIRECTION_DOWN) {
        sdlKey = SDL_SCANCODE_DOWN;
    } else if (d == DIRECTION_UP) {
        sdlKey = SDL_SCANCODE_UP;
    } else if (d == DIRECTION_LEFT) {
        sdlKey = SDL_SCANCODE_LEFT;
    } else if (d == DIRECTION_RIGHT) {
        sdlKey = SDL_SCANCODE_RIGHT;
    }

    // Move the player in a given direction.  If break out of this movement
    // loop if movePlayer returns true.  That implies that movement has been
    // interrupted.
    SDL_LockMutex(levelLoadLock);
    bool interrupt = GridLayer::GetInstance()->movePlayer(d);
    SDL_UnlockMutex(levelLoadLock);
    if (interrupt) {
        return;
    }

    // Continue moving the player as long as they are holding down an arrow
    // key.
    while (true) {

        // Check for holdDelayMs milliseconds to see if the user wishes to
        // continue moving automatically while they hold down a key.
        int x = 0;
        bool noHold = false;
        while (++x < holdDelayPollTries) {
            SDL_PumpEvents();
            if (!keyState[sdlKey]) {
                noHold = true;
                break;
            }
            SDL_Delay(holdDelayPollCheck);
        }

        // If the user is holding down the key for the provided direction ...
        if (!noHold) {

            // Update the state of the keys,
            SDL_PumpEvents();

            // While the user is still holding down the key for the provided direction...
            while (keyState[sdlKey]) {

                // Update the state of the keys
                SDL_PumpEvents();

                // This series of conditionals will allow the player to change
                // which key they are pressing without the holdDelayMs wait.

                if (keyState[SDL_SCANCODE_DOWN] && sdlKey != SDL_SCANCODE_DOWN) {
                    break;
                }

                if (keyState[SDL_SCANCODE_UP] && sdlKey != SDL_SCANCODE_UP) {
                    break;
                }

                if (keyState[SDL_SCANCODE_LEFT] && sdlKey != SDL_SCANCODE_LEFT) {
                    break;
                }

                if (keyState[SDL_SCANCODE_RIGHT] && sdlKey != SDL_SCANCODE_RIGHT) {
                    break;
                }

                // Move the player.  Exit this movement loop if the player
                // movement is interrupted (i.e. movePlayer returns true)
                SDL_LockMutex(levelLoadLock);
                bool interrupt = GridLayer::GetInstance()->movePlayer(d);
                SDL_UnlockMutex(levelLoadLock);
                if (interrupt) {
                    return;
                }


                // Wait autoMoveDelay MS before allowing another tile move.
                // This regulates player speed.
                SDL_Delay(autoMoveDelay);
            }

            // The player is no longer holding down the key for the provided
            // direction.  They have also not attempted to change directions.
            // Exit the loop.
            return;

        } else {

            // The player has chosen not to hold down the key for the provided
            // direction.  Exit the player movement loop.
            return;

        }

        // If we're here in execution, that probably means that the player has
        // changed directions during a hold.

    }
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

int KeyRunner::updateLevel(void* game) {

    KeyRunner* gameInstance = (KeyRunner*) game;

    while (gameInstance->playModel->getLevelNum() <= LevelManager::GetTotal() &&
            gameInstance->playModel->getState() != QUIT) {

        SDL_LockMutex(gameInstance->levelLoadLock);

        LevelManager::Read(gameInstance->playModel->getLevelNum());

        // Signal that it's OK to observe level tiles now.
        SDL_UnlockMutex(gameInstance->levelLoadLock);

        // Unrelated to the previous unlock, Signal every thread waiting on a
        // level to load initially that a level has been loaded.
        SDL_CondSignal(gameInstance->initialLevelLoadCond);

        SDL_LockMutex(gameInstance->levelLock);
        SDL_CondWait(gameInstance->levelCond, gameInstance->levelLock);

        gameInstance->playModel->setLevelNum(gameInstance->playModel->getLevelNum() + (uint16_t) 1);

        gameInstance->playModel->incrementTimeClock(6000);
    }

    gameInstance->exitGame();
    return 0;
}

int KeyRunner::playHandleEvents(void* game) {
    KeyRunner* gameInstance = (KeyRunner*) game;

    PlayModel* playModel = gameInstance->playModel;

    // Wait for an Event.
    SDL_Event event;
    while (playModel->getState() != QUIT) {
        SDL_WaitEvent(&event);

        if (event.type == SDL_KEYDOWN) {

            // User Presses Q
            if (event.key.keysym.sym == SDLK_q) {
                gameInstance->exitGame();
                break;

            } else if (event.key.keysym.sym == SDLK_DOWN) {
                gameInstance->moveDirection(DIRECTION_DOWN);

            } else if (event.key.keysym.sym == SDLK_UP) {
                gameInstance->moveDirection(DIRECTION_UP);

            } else if (event.key.keysym.sym == SDLK_LEFT) {
                gameInstance->moveDirection(DIRECTION_LEFT);

            } else if (event.key.keysym.sym == SDLK_RIGHT) {
                gameInstance->moveDirection(DIRECTION_RIGHT);

            }

            // If the prior movement causes the level to be complete,
            // signal that the new level may be loaded.
            if (playModel->isComplete()){
                SDL_UnlockMutex(gameInstance->levelLock);
                SDL_CondSignal(gameInstance->levelCond);
            }

        } else if (event.type == SDL_KEYUP) {

            // Handle Quit Event.
        } else if (event.type == SDL_QUIT) {
            gameInstance->exitGame();
            break;

        }
    }

    return 0;
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
