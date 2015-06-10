#include <sstream>
#include <SDL/SDL_ttf.h>

#include "KeyRunner.hpp"
#include "LevelManager.hpp"
#include "Options.hpp"
#include "../gui/AnimationFactory.hpp"
#include "../gui/ConveyorAnimation.hpp"
#include "../gui/GridLayer.hpp"
#include "../gui/EditRootLayer.hpp"
#include "../gui/PlayRootLayer.hpp"
#include "../gui/RootLayer.hpp"
#include "../model/PlayModel.hpp"
#include "../uitk/Animation.hpp"

// Items yet to be absorbed into KeyRunner static class.
Animation* KeyAnim;
Animation* PlayerAnim;

SDL_Surface* KeyRunner::screen;
SDL_mutex*   KeyRunner::screenLock;
SDL_cond*    KeyRunner::levelCond;
SDL_mutex*   KeyRunner::levelLock;
SDL_cond*    KeyRunner::levelLoadCond;
SDL_mutex*   KeyRunner::levelLoadLock;
SDL_cond*    KeyRunner::initialLevelLoadCond;
SDL_mutex*   KeyRunner::initialLevelLoadLock;
uint16_t     KeyRunner::levelNum;
State        KeyRunner::state;
int          KeyRunner::timeClock;
RootLayer*   KeyRunner::rootLayer;

void KeyRunner::play() {
    state = PLAY;

    screenLock           = SDL_CreateMutex();
    levelLock            = SDL_CreateMutex();
    levelCond            = SDL_CreateCond();
    levelLoadLock        = SDL_CreateMutex();
    levelLoadCond        = SDL_CreateCond();
    initialLevelLoadLock = SDL_CreateMutex();
    initialLevelLoadCond = SDL_CreateCond();

    levelNum = Options::getStartingLevel();
    timeClock = 50000;

    if (init()) {
        // There's not a good place for these yet.  Putting them here for now.
        KeyAnim    = AnimationFactory::Build(ANIMATION_TYPE_KEY);
        PlayerAnim = AnimationFactory::Build(ANIMATION_TYPE_PUMPKIN);

        SDL_Thread *ulThread = SDL_CreateThread(&updateLevel, NULL);

        SDL_LockMutex(initialLevelLoadLock);
        SDL_CondWait(initialLevelLoadCond, initialLevelLoadLock);

        SDL_Thread *ctThread = SDL_CreateThread(&clockTick, NULL);
        SDL_Thread *udThread = SDL_CreateThread(&updateDisplay, NULL);
        SDL_Thread *cyThread = SDL_CreateThread(&convey, NULL);

        playHandleEvents();

        SDL_WaitThread(cyThread, NULL);
        SDL_WaitThread(udThread, NULL);
        SDL_WaitThread(ctThread, NULL);
        SDL_WaitThread(ulThread, NULL);
    } else {
        // TODO: What to do if we fail to initialize?
        // Need a system for handling failures.
    }
}

void KeyRunner::edit() {
    state = EDIT;

    screenLock           = SDL_CreateMutex();
    levelLoadLock        = SDL_CreateMutex();
    levelLoadCond        = SDL_CreateCond();
    initialLevelLoadLock = SDL_CreateMutex();
    initialLevelLoadCond = SDL_CreateCond();

    if (init()) {
        // There's not a good place for these yet.  Putting them here for now.
        KeyAnim    = AnimationFactory::Build(ANIMATION_TYPE_KEY);
        PlayerAnim = AnimationFactory::Build(ANIMATION_TYPE_PUMPKIN);

        SDL_LockMutex(levelLoadLock);

        // Create New Level for Edit
        if (Options::getCreateNewLevel()) {
            levelNum = LevelManager::GetTotal() + 1;
            LevelManager::New(levelNum);

        // Load Existing Level for Edit
        } else {
            levelNum = Options::getStartingLevel();
            LevelManager::Read(levelNum);
        }

        // Signal that it's OK to observe level tiles now.
        SDL_UnlockMutex(levelLoadLock);

        // Mark all tiles as needing to be redrawn.
        GridLayer::GetInstance()->refreshTiles();

        SDL_Thread *udThread = SDL_CreateThread(&updateDisplay, NULL);

        editHandleEvents();

        SDL_WaitThread(udThread, NULL);
    } else {
        // TODO: What to do if we fail to initialize?
        // Need a system for handling failures.
    }
}

int KeyRunner::getTimeClock() {
    return timeClock;
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
    state = QUIT;
}

/* ------------------------------------------------------------------------------
 * init - Initialize the screen to 640x480x16.  Initialize the font system.
 * Set the window caption.  Initialize all layer objects.  Upon any failure,
 * return false.
 */
bool KeyRunner::init() {
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cout << "Couldn't initialize SDL: "<< SDL_GetError() << std::endl;
        return false;
    }

    if (state == EDIT) {
        rootLayer = EditRootLayer::GetInstance();
    } else {
        rootLayer = PlayRootLayer::GetInstance();
    }

    SDL_Rect rlr = rootLayer->getRect();
    screen = SDL_SetVideoMode(rlr.w, rlr.h, 16, SDL_SWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        std::cout << "Couldn't set video mode: "<< SDL_GetError() << std::endl;
        return false;
    }


    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }

    atexit(SDL_Quit);

    std::stringstream ss;
    ss << "Key Runner r" << VERSION;
    SDL_WM_SetCaption(ss.str().c_str(), "");


    return true;
}

int KeyRunner::clockTick(void* unused) {
    int step = 100;

    while (timeClock > 0 && state == PLAY) {
        SDL_Delay(step);
        timeClock -= step;
    }

    SDL_CondSignal(levelCond);
    exitGame();
    return 0;
}

/* ------------------------------------------------------------------------------
 * moveDirection - Move the player in the provided direction and pay close
 * attention to keyboard input while doing so.  This must run in the same
 * thread that initted SDL.
 *
 */
void KeyRunner::moveDirection(Direction d) {

    // Get the most recent keystate.
    Uint8* keyState = SDL_GetKeyState(NULL);

    // How long to wait in MS before assuming the user wishes to 'auto move'
    Uint16 holdDelayMs = 200;

    // How frequently to check to see if the user is holding down an arrow button.
    Uint16 holdDelayPollCheck = 10;

    // Count of poll attempts to check to see if the user is holding an arrow.
    Uint16 holdDelayPollTries = holdDelayMs/holdDelayPollCheck;

    // Number of atomic moves per second.  Each 'move' causes the player to
    // traverse one tile.
    Uint16 movesPerSecond = 10;

    // The amount of delay between tile moves.
    Uint16 autoMoveDelay = 1000/movesPerSecond;

    // Based on the provided direction, infer which key the player must be
    // pressing.
    int sdlKey = 0;
    if (d == DIRECTION_DOWN) {
        sdlKey = SDLK_DOWN;
    } else if (d == DIRECTION_UP) {
        sdlKey = SDLK_UP;
    } else if (d == DIRECTION_LEFT) {
        sdlKey = SDLK_LEFT;
    } else if (d == DIRECTION_RIGHT) {
        sdlKey = SDLK_RIGHT;
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
            keyState = SDL_GetKeyState(NULL);
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
            keyState = SDL_GetKeyState(NULL);

            // While the user is still holding down the key for the provided direction...
            while (keyState[sdlKey]) {

                // Update the state of the keys
                SDL_PumpEvents();
                keyState = SDL_GetKeyState(NULL);

                // This series of conditionals will allow the player to change
                // which key they are pressing without the holdDelayMs wait.

                if (keyState[SDLK_DOWN] && sdlKey != SDLK_DOWN) {
                    d = DIRECTION_DOWN;
                    sdlKey = SDLK_DOWN;
                    break;
                }

                if (keyState[SDLK_UP] && sdlKey != SDLK_UP) {
                    d = DIRECTION_UP;
                    sdlKey = SDLK_UP;
                    break;
                }

                if (keyState[SDLK_LEFT] && sdlKey != SDLK_LEFT) {
                    d = DIRECTION_LEFT;
                    sdlKey = SDLK_LEFT;
                    break;
                }

                if (keyState[SDLK_RIGHT] && sdlKey != SDLK_RIGHT) {
                    d = DIRECTION_RIGHT;
                    sdlKey = SDLK_RIGHT;
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
int KeyRunner::convey(void* unused) {

    // Convey only while the game has not yet been quit.
    while(state != QUIT) {

        // Don't attempt to convey if the level is being loaded.
        SDL_LockMutex(levelLoadLock);


        // Get the current tile of the player.
        TileLayer* playerTile = GridLayer::GetInstance()->getPlayerTile();

        // If the tile in a conveyor tile,
        if (playerTile->isConveyor()) {

            // Convey the player to the next tile.
            TileLayer* newTile = playerTile->getNextConveyorTile();
            if (GridLayer::GetInstance()->movePlayerToTile(newTile)) {
                if (PlayModel::GetInstance()->isComplete()){
                    SDL_UnlockMutex(levelLock);
                    SDL_CondSignal(levelCond);
                }
            }

        }

        // Indicate that it's OK to load a new level.
        SDL_UnlockMutex(levelLoadLock);

        // Delay 100 ms before conveying the player again..
        SDL_Delay(100);
    }

    return 0;
}

/* ------------------------------------------------------------------------------
 * updateDisplay - Thread.  Flip the screen 25 times per second.  Update any
 * and all animations.
 */
int KeyRunner::updateDisplay(void* unused) {
    int fps = 25;
    int delay = 1000/fps;

    while(state != QUIT) {
        SDL_LockMutex(screenLock);

        SDL_LockMutex(levelLoadLock);

        // Update and Draw the RootLayer (and all nested layers beneath).
        rootLayer->update();
        rootLayer->draw(screen);

        SDL_UnlockMutex(levelLoadLock);

        SDL_Flip(screen);

        SDL_UnlockMutex(screenLock);

        SDL_Delay(delay);
    }

    exitGame();
    return 0;
}

int KeyRunner::updateLevel(void* unused) {

    while (levelNum <= LevelManager::GetTotal() && state != QUIT) {

        SDL_LockMutex(levelLoadLock);

        LevelManager::Read(levelNum);

        // Signal that it's OK to observe level tiles now.
        SDL_UnlockMutex(levelLoadLock);

        // Unrelated to the previous unlock, Signal every thread waiting on a
        // level to load initially that a level has been loaded.
        SDL_CondSignal(initialLevelLoadCond);

        // Mark all tiles as needing to be redrawn.
        GridLayer::GetInstance()->refreshTiles();

        SDL_LockMutex(levelLock);
        SDL_CondWait(levelCond, levelLock);

        levelNum++;

        timeClock += 6000;

    }

    exitGame();
    return 0;
}

void KeyRunner::playHandleEvents() {
    // Wait for an Event.
    SDL_Event event;
    while (state != QUIT) {
        SDL_WaitEvent(&event);

        // Keydown.
        if (event.type == SDL_KEYDOWN) {

            // User Presses Q
            if (event.key.keysym.sym == SDLK_q) {
                exitGame();
                break;

            } else if (event.key.keysym.sym == SDLK_DOWN) {
                moveDirection(DIRECTION_DOWN);

            } else if (event.key.keysym.sym == SDLK_UP) {
                moveDirection(DIRECTION_UP);

            } else if (event.key.keysym.sym == SDLK_LEFT) {
                moveDirection(DIRECTION_LEFT);

            } else if (event.key.keysym.sym == SDLK_RIGHT) {
                moveDirection(DIRECTION_RIGHT);

            }

            // If the prior movement causes the level to be complete,
            // signal that the new level may be loaded.
            if (PlayModel::GetInstance()->isComplete()){
                SDL_UnlockMutex(levelLock);
                SDL_CondSignal(levelCond);
            }

        } else if (event.type == SDL_KEYUP) {

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
    while (state != QUIT) {
        SDL_WaitEvent(&event);

        // Keydown.
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

                    if (handler != NULL) {
                        handler->onKeyDown(event.key.keysym.sym);
                    }
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
