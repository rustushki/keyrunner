#include "Level.h"
#include "KeyRunner.h"
#include "InfoBar.h"

Level level;
State state;
int levelNum;

int main(int argc, char** argv) {

	// Simple Support for Starting on a Different Level.
	if (argc > 1) {
		levelNum = atoi(argv[1]);
	} else {
		levelNum  = 1;
	}

	state = PLAY;

	flipLock = SDL_CreateMutex();
	flipCond = SDL_CreateCond();

	levelLock = SDL_CreateMutex();
	levelCond = SDL_CreateCond();

	initScreen();

	timeClock = 10000;

	SDL_Thread *heThread = SDL_CreateThread(handleEvents, NULL);
	SDL_Thread *ctThread = SDL_CreateThread(clockTick, NULL);
	SDL_Thread *udThread = SDL_CreateThread(updateDisplay, NULL);

	bool firstLevelPlayed = true;
	while (levelNum <= LEVEL_COUNT && state != QUIT) {

		if (!firstLevelPlayed) {
			timeClock += 10000;
		}

		level = Level();

		level.load(levelNum);
		level.draw();

		SDL_LockMutex(levelLock);
		SDL_CondWait(levelCond, levelLock);

		firstLevelPlayed = false;

	}

	state = QUIT;

	SDL_WaitThread(udThread, NULL);
	SDL_WaitThread(ctThread, NULL);

	exitGame();

	SDL_WaitThread(heThread, NULL);


	return 0;
}
