#include "Level.h"
#include "KeyRunner.h"
#include "InfoBar.h"

Level level;
State state;

int main(int argc, char** argv) {

	// Simple Support for Starting on a Different Level.
	if (argc > 1) {
		levelNum = atoi(argv[1]);
	} else {
		levelNum  = 1;
	}

	state = PLAY;

	screenLock = SDL_CreateMutex();
	levelLock = SDL_CreateMutex();
	levelCond = SDL_CreateCond();

	timeClock = 10000;

	initScreen();

	SDL_Thread *ctThread = SDL_CreateThread(clockTick, NULL);
	SDL_Thread *udThread = SDL_CreateThread(updateDisplay, NULL);
	SDL_Thread *ulThread = SDL_CreateThread(updateLevel, NULL);

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
			SDL_mutexV(levelLock);
			SDL_CondSignal(levelCond);
		}
	}

	SDL_WaitThread(ulThread, NULL);
	SDL_WaitThread(udThread, NULL);
	SDL_WaitThread(ctThread, NULL);

	exitGame();

	return 0;
}
