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
#define PATHSEP "\\"
#elif linux
#define PATHSEP "/"
#endif

#define FONTPATH  _ROOT_ PATHSEP "font" PATHSEP "teleindicadores.ttf"
#define LEVELPATH _ROOT_ PATHSEP "level" PATHSEP
#define IMGPATH   _ROOT_ PATHSEP "img" PATHSEP

#define VERSION 3

#include "../model/Direction.hpp"
#include "../model/PlayModel.hpp"
#include "../model/State.hpp"

class Animation;
class RootLayer;

extern Animation* KeyAnim;
extern Animation* PlayerAnim;

class KeyRunner {
public:
    static void play();
    static void edit();
    static void exitGame();
    static int getTimeClock();
    static RootLayer* getRootLayer();

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
    static SDL_mutex*  screenLock;
    static SDL_cond*   levelCond;
    static SDL_mutex*  levelLock;
    static SDL_cond*   levelLoadCond;
    static SDL_mutex*  levelLoadLock;
    static SDL_cond*   initialLevelLoadCond;
    static SDL_mutex*  initialLevelLoadLock;
    static State       state;
    static int         timeClock;
    static RootLayer*  rootLayer;
    static PlayModel*  playModel;
};

#endif//KEYRUNNER_HPP
