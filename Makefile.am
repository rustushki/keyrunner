SUBDIRS = level img
EXTRA_DIST = img level svg

gamesdir = $(prefix)/games

games_PROGRAMS = KeyRunner

KeyRunner_SOURCES =  \
InfoBar.cpp          \
InfoBar.h            \
KeyRunner.cpp        \
KeyRunner.h          \
Level.cpp            \
Level.h              \
main.cpp

KeyRunner_LDADD =    \
$(SDL_LIBS)          \
$(SDL_image_LIBS)    \
$(SDL_ttf_LIBS)      \
$(libpng_LIBS)
