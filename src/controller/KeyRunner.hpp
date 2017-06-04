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

class Animation;
class RootLayer;

extern Animation* KeyAnim;
extern Animation* PlayerAnim;

class KeyRunner {
public:
    void play();
    void edit();
    void exitGame();
    RootLayer* getRootLayer();

private:
    // Thread functions.
    static int clockTick(void* game);
    static int updateDisplay(void* game);
    static int updateLevel(void* game);
    static int convey(void* game);

    // Helpers
    bool init();
    void moveDirection(Direction d);
    void playHandleEvents();
    void editHandleEvents();

    SDL_Surface *screen;
    SDL_mutex*  screenLock;
    SDL_cond*   levelCond;
    SDL_mutex*  levelLock;
    SDL_cond*   levelLoadCond;
    SDL_mutex*  levelLoadLock;
    SDL_cond*   initialLevelLoadCond;
    SDL_mutex*  initialLevelLoadLock;
    RootLayer*  rootLayer;
    PlayModel*  playModel;
};

#endif//KEYRUNNER_HPP
