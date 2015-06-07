gamesdir = $(prefix)/games

games_PROGRAMS = keyrunner

keyrunner_SOURCES =    \
Animation.cpp          \
ButtonLayer.cpp        \
ButtonLayerBuilder.cpp \
ConveyorAnimation.cpp  \
EditInfoBarLayer.cpp   \
EditRootLayer.cpp      \
GridLayer.cpp          \
InfoBarLayer.cpp       \
KeyRunner.cpp          \
Layer.cpp              \
LevelManager.cpp       \
Options.cpp            \
PlayInfoBarLayer.cpp   \
PlayRootLayer.cpp      \
RootLayer.cpp          \
SpriteSheet.cpp        \
TileLayer.cpp          \
TileSelectorLayer.cpp  \
main.cpp


keyrunner_LDADD =    \
$(SDL_LIBS)          \
$(SDL_image_LIBS)    \
$(SDL_ttf_LIBS)      \
$(libpng_LIBS)       \
$(liblzma_LIBS)

AM_CPPFLAGS= -O0 -D '_ROOT_="$(prefix)/share/keyrunner"'
