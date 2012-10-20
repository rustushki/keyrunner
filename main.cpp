#include "Level.h"
#include "KeyRunner.h"
#include "InfoBar.h"

#define LEVEL_COUNT 5

Level level;
State state;
int levelNum;

int main() {

	state = PLAY;

	flipLock = SDL_CreateMutex();
	flipCond = SDL_CreateCond();

	initScreen();

	timeClock = 10000;
	levelNum  = 1;

	SDL_Thread *heThread = SDL_CreateThread(handleEvents, NULL);
	SDL_Thread *ctThread = SDL_CreateThread(clockTick, NULL);
	SDL_Thread *udThread = SDL_CreateThread(updateDisplay, NULL);

	while (levelNum <= LEVEL_COUNT && state != QUIT) {

		level = Level();

		level.load(levelNum);
		level.draw();


		timeClock += 10000;
		
	}

	SDL_WaitThread(udThread, NULL);
	SDL_WaitThread(ctThread, NULL);

	exitGame();

	SDL_WaitThread(heThread, NULL);


	return 0;
}
