#ifndef KEY_RUNNER_HPP
#define KEY_RUNNER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdint>

#ifndef _ROOT_
#define _ROOT_ "."
#endif

#if _WIN32
#define PATHSEP "\\"
#elif linux
#define PATHSEP "/"
#endif

#define FONT_TELEINDICADORES  _ROOT_ PATHSEP "font" PATHSEP "teleindicadores.ttf"
#define FONT_CELTIC_HAND _ROOT_ PATHSEP "font" PATHSEP "celtic_hand.ttf"
#define LEVEL_PATH _ROOT_ PATHSEP "level" PATHSEP
#define IMAGE_PATH _ROOT_ PATHSEP "img" PATHSEP

#define VERSION "1.4.1"

#endif
