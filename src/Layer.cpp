#include <SDL/SDL.h>

#include <iostream>
#include "Layer.hpp"

Layer* Layer::selectedLayer = NULL;

Layer::Layer() {
    onEnterCb = NULL;
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
    subLayer->setParent(this);
    subLayers.push_back(subLayer);
}

std::function<void()> Layer::getOnEnterCb() const {
    return onEnterCb;
}

void Layer::onEnter() {
    // Handle the enter press with the Layer's onEnter handler (if there is one).
    std::function<void()> onEnterCb = getOnEnterCb();
    if (onEnterCb != NULL) {
        onEnterCb();
    }

    // Iterate over the sub layers backwards so that the uppermost stacked are
    // evaluated first.  The first layer from the top which contains the point
    // will propagate the onEnter event.
    std::vector<Layer*>::reverse_iterator itr;
    for (itr = subLayers.rbegin(); itr < subLayers.rend(); itr++) {
        (*itr)->onEnter();
        break;
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
    setSelected();
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

/* ------------------------------------------------------------------------------
 * getSelectedChildIndex - Return the index of the child Layer which is
 * selected.  Returns negative number if there is not one selected.
 */
int Layer::getSelectedChildIndex() {
    for (uint16_t x = 0; x < getChildCount(); x++) {
        if (getChild(x)->isSelected()) {
            return x;
        }
    }

    return -1;
}

/* ------------------------------------------------------------------------------
 * getChildCount - Return the total count of child layers.
 */
uint16_t Layer::getChildCount() {
    return subLayers.size();
}

/* ------------------------------------------------------------------------------
 * isSelected - Return whether or not this Layer is the one that is selected.
 */
bool Layer::isSelected() const {
    return (Layer::selectedLayer == this);
}

/* ------------------------------------------------------------------------------
 * setSelected - Makes this layer selected.
 */
void Layer::setSelected() {
    selectedLayer = this;
    onSelected();
}

/* ------------------------------------------------------------------------------
 * onSelected - Called when selection is set to this Layer.  Default behavior
 * is to grant selection to the first child.  Override to change.
 */
void Layer::onSelected() {
    if (subLayers.size() > 0) {
        subLayers[0]->setSelected();
    }
}

/* ------------------------------------------------------------------------------
 * setParent - set the parent Layer of this Layer.
 */
void Layer::setParent(Layer* parent) {
    this->parent = parent;
}
