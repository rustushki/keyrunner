#include <SDL/SDL.h>

#include "GridLayer.hpp"
#include "InfoBarLayer.hpp"
#include "RootLayer.hpp"

RootLayer* RootLayer::instance = 0;

RootLayer::RootLayer() {
    addLayer(GridLayer::GetInstance());
    addLayer(InfoBarLayer::GetInstance());
}

RootLayer* RootLayer::GetInstance() {
    if (instance == 0) {
        instance = new RootLayer();
    }

    return instance;
}

void RootLayer::draw(SDL_Surface* dst) {
    Layer::draw(dst);
}

SDL_Rect RootLayer::getRect() const {
    SDL_Rect glr = GridLayer::GetInstance()->getRect();
    SDL_Rect iblr = InfoBarLayer::GetInstance()->getRect();

    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = glr.w;
    r.h = glr.h + iblr.h;
    return r;
}

void RootLayer::update() {
    Layer::update();
}
