#ifndef KEYRUNNER_H
#define KEYRUNNER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <stdint.h>

#ifndef _ROOT_
#define _ROOT_ "."
#endif

#if _WIN32
#define FONTPATH  _ROOT_ "\\font\\teleindicadores.ttf"
#define LEVELPATH _ROOT_ "\\level\\"
#define IMGPATH   _ROOT_ "\\img\\"
#elif linux
#define FONTPATH  _ROOT_ "/font/teleindicadores.ttf"
#define LEVELPATH _ROOT_ "/level/"
#define IMGPATH   _ROOT_ "/img/"
#endif

#define VERSION 3

#include "Direction.h"

class Level;
class Animation;

// Game State.
enum State {
	  PLAY     // User is playing.
	, QUIT     // User has indicated he wants to quit.
	, START    // Build the next level.
	, LOSE     // User has lost.  Display lose animation.
	, RESTART  // Start over at level 1, reset score, etc..
	, WIN      // User has won the game.
};

extern SDL_Surface *screen;
extern Level level;
extern int timeClock;

extern State state;

extern Animation* KeyAnim;
extern Animation* PlayerAnim;

int getWidth();
int getHeight();
void exitGame();

class KeyRunner {
public:
	static void play(uint16_t startLevel);

private:
	// Thread functions.
	static int clockTick(void* unused);
	static int updateDisplay(void* unused);
	static int updateLevel(void* unused);
	static int convey(void* unused);

	// Helpers
	static bool init();
	static void moveDirection(Direction d);
	static void handleEvents();

	static uint16_t levelNum;
	static SDL_mutex* screenLock;
	static SDL_cond* levelCond;
	static SDL_mutex* levelLock;
	static SDL_cond* levelLoadCond;
	static SDL_mutex* levelLoadLock;
	static SDL_cond* initialLevelLoadCond;
	static SDL_mutex* initialLevelLoadLock;

};

#endif//KEYRUNNER_H
