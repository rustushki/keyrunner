#ifndef KEYRUNNER_H
#define KEYRUNNER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>

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

typedef unsigned int uint;

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

extern SDL_mutex* screenLock;

extern SDL_cond* levelCond;
extern SDL_mutex* levelLock;
extern SDL_cond* levelLoadCond;
extern SDL_mutex* levelLoadLock;
extern SDL_cond* initialLevelLoadCond;
extern SDL_mutex* initialLevelLoadLock;

extern State state;
extern unsigned int levelNum;

extern bool flipping;
extern bool blitting;

extern Animation* KeyAnim;
extern Animation* PlayerAnim;

void moveDirection(Direction d);
int getWidth();
int getHeight();
void exitGame();
bool init();
int clockTick(void* unused);
int updateDisplay(void* unused);
int updateLevel(void* unused);
int convey(void* unused);

#endif//KEYRUNNER_H
