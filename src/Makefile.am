gamesdir = $(prefix)/games

games_PROGRAMS = keyrunner

keyrunner_SOURCES =  \
Animation.cpp        \
Animation.h          \
AnimationType.h      \
InfoBar.cpp          \
InfoBar.h            \
KeyRunner.cpp        \
KeyRunner.h          \
Level.cpp            \
Level.h              \
SpriteSheet.cpp      \
SpriteSheet.h        \
main.cpp


keyrunner_LDADD =    \
$(SDL_LIBS)          \
$(SDL_image_LIBS)    \
$(SDL_ttf_LIBS)      \
$(libpng_LIBS)

AM_CPPFLAGS= -D '_ROOT_="$(prefix)/share/keyrunner"'
