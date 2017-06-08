#include <SDL2/SDL.h>

#include "GridLayer.hpp"
#include "InfoBarLayer.hpp"
#include "RootLayer.hpp"

RootLayer::RootLayer(InfoBarLayer* ibl) {
    addLayer(GridLayer::GetInstance());
    addLayer(ibl);
    this->ibl = ibl;
    this->ibl->setSelected();
}

void RootLayer::draw(SDL_Renderer* renderer) {
    Layer::draw(renderer);
}

SDL_Rect RootLayer::getRect() const {
    SDL_Rect glr = GridLayer::GetInstance()->getRect();
    SDL_Rect infoBarLayerRect = ibl->getRect();

    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = glr.w;
    r.h = glr.h + infoBarLayerRect.h;
    return r;
}
