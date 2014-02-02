#include <SDL/SDL.h>

#include "Layer.hpp"

void Layer::draw(SDL_Surface* dst) {
    std::vector<Layer*>::iterator itr;
    for (itr = subLayers.begin(); itr < subLayers.end(); itr++) {
        (*itr)->draw(dst);
    }
}

void Layer::update() {
    std::vector<Layer*>::iterator itr;
    for (itr = subLayers.begin(); itr < subLayers.end(); itr++) {
        (*itr)->update();
    }
}

void Layer::addLayer(Layer* subLayer) {
    subLayers.push_back(subLayer);
}

void Layer::onClick(uint16_t x, uint16_t y) {
    std::vector<Layer*>::iterator itr;
    for (itr = subLayers.begin(); itr < subLayers.end(); itr++) {
        if ((*itr)->contains(x, y)) {
            (*itr)->onClick(x, y);
        }
    }
}

bool Layer::contains(uint16_t x, uint16_t y) {
    bool ret = false;

    SDL_Rect r = getRect();
    if (r.x <= x && r.x + r.w >= x) {
        if (r.y <= y && r.y + r.w >= y) {
            ret = true;
        }
    }

    return ret;
}
