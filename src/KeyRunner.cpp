#include "Animation.h"
#include "ConveyorAnimation.h"
#include "KeyRunner.h"
#include "InfoBar.h"
#include "Level.h"
#include <sstream>
#include <SDL/SDL_ttf.h>

SDL_Surface *screen;
int timeClock;
SDL_mutex* screenLock;

SDL_cond* levelCond;
SDL_mutex* levelLock;
SDL_cond* levelLoadCond;
SDL_mutex* levelLoadLock;
SDL_mutex* initialLevelLoadLock;

Animation* KeyAnim;
Animation* PlayerAnim;

uint levelNum;
uint frame;


int getWidth() {
	return GRID_WIDTH*25;
}

int getHeight() {
	return GRID_HEIGHT*25 + InfoBar::GetInstance()->getHeight();
}

void exitGame() {
	SDL_Event quitEvent;
	quitEvent.type = SDL_QUIT;
	SDL_PushEvent(&quitEvent);
	state = QUIT;
}

/* ------------------------------------------------------------------------------
 * initScreen - Initialize the screen to 640x480x16.  Initialize the font
 * system.  Set the window caption.  Upon any failure, return false.
 */
bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "Couldn't initialize SDL: "<< SDL_GetError() << std::endl;
		return false;
	}

	screen = SDL_SetVideoMode(getWidth(), getHeight(), 16, SDL_SWSURFACE | SDL_DOUBLEBUF);
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

	screen = SDL_SetVideoMode(getWidth(), getHeight(), 16, SDL_SWSURFACE | SDL_DOUBLEBUF);
	std::stringstream ss;
	ss << "Key Runner r" << VERSION;
	SDL_WM_SetCaption(ss.str().c_str(), "");
	return true;
}

int clockTick(void* unused) {

	InfoBar* ib = InfoBar::GetInstance();

	int step = 100;

	while (timeClock > 0 && state == PLAY) {
		SDL_Delay(step);
		timeClock -= step;
		ib->draw();
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
void moveDirection(Direction d) {

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
	if (level.movePlayer(d)) {
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
				if (level.movePlayer(d)) {
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
int convey(void* unused) {

	SDL_LockMutex(initialLevelLoadLock);

	// Convey only while the game has not yet been quit.
	while(state != QUIT) {

		// Don't attempt to convey if the level is being loaded.
		SDL_LockMutex(levelLoadLock);


		// Get the current tile of the player.
		Tile* playerTile = level.getPlayerTile();

		// If the tile in a conveyor tile,
		if (playerTile->isConveyor()) {

			// Convey the player to the next tile.
			Tile* newTile = playerTile->getNextConveyorTile();
			level.movePlayerToTile(newTile);

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
int updateDisplay(void* unused) {
	int fps = 25;

	int delay = 1000/fps;
	while(state != QUIT) {

		SDL_LockMutex(levelLoadLock);
		ConveyorAnimation::StartConveyors();
		Tile::AnimateTiles();
		Tile::RedrawChangedTiles();
		SDL_UnlockMutex(levelLoadLock);

		SDL_LockMutex(screenLock);

		SDL_Flip(screen);

		SDL_UnlockMutex(screenLock);

		SDL_Delay(delay);
	}

	exitGame();
	return 0;
}

int updateLevel(void* unused) {

	while (levelNum <= Level::GetTotal() && state != QUIT) {

		SDL_LockMutex(levelLoadLock);

		Tile::ClearChangedTiles();

		level = Level();

		level.load(levelNum);

		// Signal that it's OK to observe level tiles now.
		SDL_UnlockMutex(levelLoadLock);

		// Unrelated to the previous unlock, Signal every thread waiting on a
		// level to load initially that a level has been loaded.
		SDL_UnlockMutex(initialLevelLoadLock);

		level.draw();

		SDL_LockMutex(levelLock);
		SDL_CondWait(levelCond, levelLock);

		levelNum++;

		Tile::ClearAnimatedTiles();

		timeClock += 6000;

	}

	exitGame();
	return 0;
}
