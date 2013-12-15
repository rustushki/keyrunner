#include <SDL/SDL.h>

#include "GridLayer.hpp"
#include "InfoBarLayer.hpp"
#include "PlayInfoBarLayer.hpp"
#include "EditInfoBarLayer.hpp"
#include "RootLayer.hpp"

RootLayer::RootLayer(InfoBarLayer* ibl) {
    this->ibl = ibl;
    addLayer(GridLayer::GetInstance());
    addLayer(ibl);
}

void RootLayer::draw(SDL_Surface* dst) {
    Layer::draw(dst);
}

SDL_Rect RootLayer::getRect() const {
    SDL_Rect glr = GridLayer::GetInstance()->getRect();
    SDL_Rect iblr = ibl->getRect();

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
