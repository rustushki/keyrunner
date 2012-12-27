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

	exit(1);
}

/* ------------------------------------------------------------------------------
 * initScreen - Initialize the screen to 640x480x16. Exit game on failure.
 */
void initScreen() {
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "Couldn't initialize SDL: "<< SDL_GetError() << std::endl;
		exitGame();
	}

	atexit(SDL_Quit);

	screen = SDL_SetVideoMode(getWidth(), getHeight(), 16, SDL_SWSURFACE | SDL_DOUBLEBUF);
	if (screen == NULL) {
		std::cout << "Couldn't set video mode: "<< SDL_GetError() << std::endl;
		exitGame();
    }


	// Initialize SDL_ttf
	if (TTF_Init() == -1) {
		std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
	}

	std::stringstream ss;
	ss << "Key Runner r" << VERSION;
	SDL_WM_SetCaption(ss.str().c_str(), "");
}

int clockTick(void* unused) {

	InfoBar* ib = InfoBar::GetInstance();

	int step = 100;

	while (timeClock > 0 && state == PLAY) {
		SDL_Delay(step);
		timeClock -= step;
		ib->draw();
	}

	state = QUIT;

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
 * updateDisplay - Thread.  Flip the screen 25 times per second.  Update any
 * and all animations.
 */
int updateDisplay(void* unused) {
	int fps = 25;

	int delay = 1000/fps;
	while(state != QUIT) {

		ConveyorAnimation::StartConveyors();

		Tile::AnimateTiles();
		Tile::RedrawChangedTiles();

		SDL_mutexP(screenLock);

		SDL_Flip(screen);

		SDL_mutexV(screenLock);

		SDL_Delay(delay);
	}

	exitGame();
	return 0;
}

int updateLevel(void* unused) {

	bool firstLevelPlayed = true;
	while (levelNum <= Level::GetTotal() && state != QUIT) {

		if (!firstLevelPlayed) {
			timeClock += 10000;
		}

		level = Level();

		level.load(levelNum);
		level.draw();

		SDL_LockMutex(levelLock);
		SDL_CondWait(levelCond, levelLock);

		levelNum++;

		Tile::ClearAnimatedTiles();

		firstLevelPlayed = false;

	}

	state = QUIT;
	exitGame();

	return 0;
}
