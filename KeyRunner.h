#ifndef KEYRUNNER_H
#define KEYRUNNER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>

#define LEVEL_COUNT 15
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
extern SDL_mutex* flipLock;
extern SDL_cond* levelCond;
extern SDL_mutex* levelLock;

extern State state;
extern int levelNum;

int getWidth();
int getHeight();
void exitGame();
void initScreen();
int handleEvents(void* unused);
int clockTick(void* unused);
int updateDisplay(void* unused);

#endif//KEYRUNNER_H
