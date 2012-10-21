#include "KeyRunner.h"
#include "InfoBar.h"
#include "Level.h"
#include <SDL/SDL_ttf.h>

SDL_Surface *screen;
int timeClock;
SDL_cond* flipCond;
SDL_mutex* flipLock;

SDL_cond* levelCond;
SDL_mutex* levelLock;

int getWidth() {
	return GRID_WIDTH*25;
}

int getHeight() {
	return GRID_HEIGHT*25 + InfoBar::GetInstance()->getHeight();
}

void exitGame() {
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
}

/* ------------------------------------------------------------------------------
 * handleEvents - Handle events from the user.
 */
int handleEvents(void* unused) {

	// Wait for an Event.
	SDL_Event event;
	while (SDL_WaitEvent(&event)) {

		// Keydown.
		if (event.type == SDL_KEYDOWN) {

			// User Presses Q
			if (event.key.keysym.sym == SDLK_q) {
				
				// Quit Game.
				exitGame();

			} else if (event.key.keysym.sym == SDLK_DOWN) {
				level.movePlayer(DIRECTION_DOWN);
			} else if (event.key.keysym.sym == SDLK_UP) {
				level.movePlayer(DIRECTION_UP);
			} else if (event.key.keysym.sym == SDLK_LEFT) {
				level.movePlayer(DIRECTION_LEFT);
			} else if (event.key.keysym.sym == SDLK_RIGHT) {
				level.movePlayer(DIRECTION_RIGHT);
			}


		// Handle Quit Event.
		} else if (event.type == SDL_QUIT) {

			exitGame();
		}


		if (level.isComplete()) {
			levelNum++;
			SDL_CondSignal(levelCond);
		}
	}
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

	return 0;
}

int updateDisplay(void* unused) {
	int fps = 25;

	SDL_mutex *flipLock = SDL_CreateMutex();

	int delay = 1000/fps;
	while(state != QUIT) {

		// Safely flip the screen.
		SDL_CondWait(flipCond, flipLock);
		SDL_Flip(screen);
		SDL_CondSignal(flipCond);

		SDL_Delay(delay);
	}

	return 0;
}
