#ifndef KEYRUNNER_HPP
#define KEYRUNNER_HPP

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

#include "Direction.hpp"
#include "State.hpp"

class Animation;
class Level;
class RootLayer;

extern Animation* KeyAnim;
extern Animation* PlayerAnim;

class KeyRunner {
public:
    static void play();
    static void edit();
    static void exitGame();
    static int getTimeClock();
    static uint16_t getLevelNum();
    static RootLayer* getRootLayer();
    static Level* getCurrentLevel();

private:
    // Thread functions.
    static int clockTick(void* unused);
    static int updateDisplay(void* unused);
    static int updateLevel(void* unused);
    static int convey(void* unused);

    // Helpers
    static bool init();
    static void moveDirection(Direction d);
    static void playHandleEvents();
    static void editHandleEvents();

    static SDL_Surface *screen;
    static uint16_t    levelNum;
    static SDL_mutex*  screenLock;
    static SDL_cond*   levelCond;
    static SDL_mutex*  levelLock;
    static SDL_cond*   levelLoadCond;
    static SDL_mutex*  levelLoadLock;
    static SDL_cond*   initialLevelLoadCond;
    static SDL_mutex*  initialLevelLoadLock;
    static State       state;
    static int         timeClock;
    static Level*      level;
    static RootLayer*  rootLayer;
};

#endif//KEYRUNNER_HPP
