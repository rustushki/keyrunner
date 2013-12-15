gamesdir = $(prefix)/games

games_PROGRAMS = keyrunner

keyrunner_SOURCES =   \
Animation.cpp         \
ConveyorAnimation.cpp \
EditInfoBarLayer.cpp  \
EditRootLayer.cpp     \
InfoBarLayer.cpp      \
GridLayer.cpp         \
KeyRunner.cpp         \
Layer.cpp             \
Level.cpp             \
LevelLoader.cpp       \
Options.cpp           \
PlayInfoBarLayer.cpp  \
PlayRootLayer.cpp     \
RootLayer.cpp         \
SpriteSheet.cpp       \
Tile.cpp              \
main.cpp


keyrunner_LDADD =    \
$(SDL_LIBS)          \
$(SDL_image_LIBS)    \
$(SDL_ttf_LIBS)      \
$(libpng_LIBS)

AM_CPPFLAGS= -O0 -D '_ROOT_="$(prefix)/share/keyrunner"'
