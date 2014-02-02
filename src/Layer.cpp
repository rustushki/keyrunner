#include <SDL/SDL.h>

#include "Layer.hpp"

Layer::Layer() {
    onClickCb = NULL;
}

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

std::function<void()> Layer::getOnClickCb() const {
    return onClickCb;
}

void Layer::onClick(uint16_t x, uint16_t y) {
    // Handle the click with the Layer's onClick handler (if there is one).
    std::function<void()> onClickCb = getOnClickCb();
    if (onClickCb != NULL) {
        onClickCb();
    }

    // Iterate over child Layers and propagate the event if they contain the
    // point.
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

void Layer::setOnClick(std::function<void()> cb) {
    onClickCb = cb;
}
