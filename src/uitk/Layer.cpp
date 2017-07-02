#include <SDL2/SDL.h>

#include <iostream>
#include "../uitk/Layer.hpp"

Layer::Layer() {
    onEnterCb = nullptr;
    parent = nullptr;
    show();
}

void Layer::draw(SDL_Renderer* renderer) {
    std::vector<Layer*>::iterator itr;
    for (itr = subLayers.begin(); itr < subLayers.end(); itr++) {
        if ((*itr)->isVisible()) {
            (*itr)->draw(renderer);
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
    subLayer->setParent(this);
    subLayers.push_back(subLayer);
}

std::function<void()> Layer::getOnEnterCb() const {
    return onEnterCb;
}

void Layer::onEnter() {
    // Handle the enter press with the Layer's onEnter handler (if there is one).
    std::function<void()> onEnterCb = getOnEnterCb();
    if (onEnterCb != nullptr) {
        onEnterCb();
    }

    // Get the layer on top and propagate the enter keypress to it
    if (subLayers.size()) {
        subLayers.back()->onEnter();
    }
}

void Layer::onKeyDown(SDL_Keycode key) {
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

void Layer::setOnEnter(std::function<void()> cb) {
    onEnterCb = cb;
}

/* ------------------------------------------------------------------------------
 * isVisible - Whether or not the layer should be drawn onto its parent layer.
 */
bool Layer::isVisible() {
    return visible;
}

/* ------------------------------------------------------------------------------
 * show - Make the layer visible.  Note that layers default to visible.
 * Calling show will automatically give this widget selection.
 */
void Layer::show() {
    visible = true;
    //setSelected();
}

/* ------------------------------------------------------------------------------
 * hide - Make the layer invisible.  Note that layers default to visible, so a
 * call must be made to hide() in order to make them invisible.  Give the
 * parent focus.
 */
void Layer::hide() {
    visible = false;
    if (getParent() != nullptr) {
        //getParent()->setSelected();
    }
}

/* ------------------------------------------------------------------------------
 * getChild - Get the Nth child layer of this layer.  Return nullptr if there
 * isn't one.
 */
Layer* Layer::getChild(int i) {
    return subLayers[i];
}

/* ------------------------------------------------------------------------------
 * getChildCount - Return the total count of child layers.
 */
uint16_t Layer::getChildCount() {
    return (uint16_t) subLayers.size();
}

/* ------------------------------------------------------------------------------
 * getParent - Return the parent layer.
 */
Layer* Layer::getParent() const {
    return parent;
}

/* ------------------------------------------------------------------------------
 * setParent - set the parent Layer of this Layer.
 */
void Layer::setParent(Layer* parent) {
    this->parent = parent;
}
