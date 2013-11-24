#include "Animation.h"
#include "Level.h"
#include "KeyRunner.h"

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

	screenLock           = SDL_CreateMutex();
	levelLock            = SDL_CreateMutex();
	levelCond            = SDL_CreateCond();
	levelLoadLock        = SDL_CreateMutex();
	levelLoadCond        = SDL_CreateCond();
	initialLevelLoadLock = SDL_CreateMutex();
	initialLevelLoadCond = SDL_CreateCond();

	timeClock = 50000;

	if (init()) {
		// There's not a good place for these yet.  Putting them here for now.
		KeyAnim    = Animation::AnimationFactory(ANIMATION_TYPE_KEY);
		PlayerAnim = Animation::AnimationFactory(ANIMATION_TYPE_PUMPKIN);

		SDL_Thread *ctThread = SDL_CreateThread(clockTick, NULL);
		SDL_Thread *udThread = SDL_CreateThread(updateDisplay, NULL);
		SDL_Thread *ulThread = SDL_CreateThread(updateLevel, NULL);
		SDL_Thread *cyThread = SDL_CreateThread(convey, NULL);

		handleEvents();

		SDL_WaitThread(cyThread, NULL);
		SDL_WaitThread(ulThread, NULL);
		SDL_WaitThread(udThread, NULL);
		SDL_WaitThread(ctThread, NULL);
	}

	return 0;
}
