#include <SDL/SDL.h>

#include <iostream>
#include "Layer.hpp"

Layer::Layer() {
    onClickCb = NULL;
    show();
}

void Layer::draw(SDL_Surface* dst) {
    std::vector<Layer*>::iterator itr;
    for (itr = subLayers.begin(); itr < subLayers.end(); itr++) {
        if ((*itr)->isVisible()) {
            (*itr)->draw(dst);
        }
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

    // Iterate over the sub layers backwards so that the uppermost stacked are
    // evaluated first.  The first layer from the top which contains the point
    // will propagate the onClick event.
    std::vector<Layer*>::reverse_iterator itr;
    for (itr = subLayers.rbegin(); itr < subLayers.rend(); itr++) {
        if ((*itr)->contains(x, y) && (*itr)->isVisible()) {
            (*itr)->onClick(x, y);
            break;
        }
    }
}

void Layer::onKeyDown(SDLKey key) {
    std::vector<Layer*>::iterator itr;
    for (itr = subLayers.begin(); itr < subLayers.end(); itr++) {
        (*itr)->onKeyDown(key);
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

/* ------------------------------------------------------------------------------
 * isVisible - Whether or not the layer should be drawn onto its parent layer.
 */
bool Layer::isVisible() {
    return visible;
}

/* ------------------------------------------------------------------------------
 * show - Make the layer visible.  Note that layers default to visible.
 */
void Layer::show() {
    visible = true;
}

/* ------------------------------------------------------------------------------
 * hide - Make the layer invisible.  Note that layers default to visible, so a
 * call must be made to hide() in order to make them invisible.
 */
void Layer::hide() {
    visible = false;
}

/* ------------------------------------------------------------------------------
 * getChild - Get the Nth child layer of this layer.  Return NULL if there
 * isn't one.
 */
Layer* Layer::getChild(int i) {
    return subLayers[i];
}
