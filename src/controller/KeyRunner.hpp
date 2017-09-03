#ifndef KEY_RUNNER_HPP
#define KEY_RUNNER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdint>

// By default, the application data root will be the current working directory
#define APPLICATION_DATA_ROOT "."

// If Snapcraft was the builder, the application data root will be set by the SNAP environment variable
#ifdef BUILDER_SNAP
#define APPLICATION_DATA_ROOT std::string(std::getenv("SNAP"))
#endif

#ifdef CMAKE_INSTALL_PREFIX
#define APPLICATION_DATA_ROOT CMAKE_INSTALL_PREFIX "/share/keyrunner"
#endif

#if _WIN32
#define PATHSEP "\\"
#elif linux
#define PATHSEP "/"
#endif

#define FONT_TELEINDICADORES  APPLICATION_DATA_ROOT PATHSEP "font" PATHSEP "teleindicadores.ttf"
#define FONT_CELTIC_HAND APPLICATION_DATA_ROOT PATHSEP "font" PATHSEP "celtic_hand.ttf"
#define LEVEL_PATH APPLICATION_DATA_ROOT PATHSEP "level" PATHSEP
#define IMAGE_PATH APPLICATION_DATA_ROOT PATHSEP "img" PATHSEP

#define VERSION "1.4.1"

#endif
