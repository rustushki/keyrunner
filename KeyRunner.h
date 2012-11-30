#ifndef KEYRUNNER_H
#define KEYRUNNER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>

#if _WIN32
#define FONTPATH "\\windows\\fonts\\verdana.ttf"
#define LEVELPATH ".\\level\\"
#define IMGPATH ".\\img\\"
#elif linux
#define FONTPATH "/usr/share/fonts/truetype/msttcorefonts/verdana.ttf"
#define LEVELPATH "./level/"
#define IMGPATH "./img/"
#endif

#define VERSION 2

class Level;

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

extern SDL_cond* flipCond;
extern SDL_cond* blitCond;
extern SDL_mutex* flipLock;
extern SDL_cond* levelCond;
extern SDL_mutex* levelLock;

extern State state;
extern int levelNum;

extern bool flipping;
extern bool blitting;

int getWidth();
int getHeight();
void exitGame();
void initScreen();
int clockTick(void* unused);
int updateDisplay(void* unused);
int updateLevel(void* unused);

#endif//KEYRUNNER_H
