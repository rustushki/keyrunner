#ifndef KEY_RUNNER_HPP
#define KEY_RUNNER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

#define FONT_PATH  _ROOT_ PATHSEP "font" PATHSEP "teleindicadores.ttf"
#define LEVEL_PATH _ROOT_ PATHSEP "level" PATHSEP
#define IMAGE_PATH _ROOT_ PATHSEP "img" PATHSEP

#define VERSION 3

#include "../model/Direction.hpp"
#include "../model/PlayModel.hpp"

class Animation;
class RootLayer;

extern Animation* KeyAnimation;
extern Animation* PlayerAnimation;

class KeyRunner {
public:
    void play();
    void edit();
    void exitGame();
    RootLayer* getRootLayer();

private:
    void updateDisplay();

    // Thread functions.
    static int clockTick(void* game);
    static int updateLevel(void* game);
    static int convey(void* game);

    // Helpers
    bool init();
    void moveDirection(Direction d);
    void editHandleEvents();
    void processInput();

    SDL_Window*   window;
    SDL_Renderer* renderer;
    SDL_cond*     levelCond;
    SDL_mutex*    levelLock;
    SDL_mutex*    levelLoadLock;
    SDL_cond*     initialLevelLoadCond;
    SDL_mutex*    initialLevelLoadLock;
    RootLayer*    rootLayer;
    PlayModel*    playModel;
};

#endif
