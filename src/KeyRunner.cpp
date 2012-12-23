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
