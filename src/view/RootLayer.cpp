#include <SDL2/SDL.h>

#include "GridLayer.hpp"
#include "InfoBarLayer.hpp"
#include "PlayInfoBarLayer.hpp"
#include "EditInfoBarLayer.hpp"
#include "RootLayer.hpp"

RootLayer::RootLayer(InfoBarLayer* ibl) {
    addLayer(GridLayer::GetInstance());
    addLayer(ibl);
    this->ibl = ibl;
    this->ibl->setSelected();
}

void RootLayer::draw(SDL_Renderer* renderer, SDL_Texture* dst) {
    Layer::draw(renderer, dst);
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
