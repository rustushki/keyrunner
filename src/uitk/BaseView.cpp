#include "../uitk/BaseView.hpp"

/**
 * Constructor.
 * <p>
 * Store the model and rectangle region of this View.
 * @param model where to find data to display
 * @param rect where to display on the screen
 */
BaseView::BaseView(PlayModel *model, SDL_Rect rect) {
    this->model = model;
    this->rect = rect;
}

/**
 * Get the model that contains the information to display.
 * @return PlayModel*
 */
PlayModel* BaseView::getModel() const {
    return this->model;
}

/**
 * Get the rectangular region that this View will be drawn to.
 * @return
 */
SDL_Rect BaseView::getRect() const {
    return rect;
}

/**
 * Make this view visible.
 */
void BaseView::show() {
    this->visible = true;
}

/**
 * Make this view invisible.
 */
void BaseView::hide() {
    this->visible = false;
}

/**
 * Is this View visible?
 * @return boolean
 */
bool BaseView::isVisible() const {
    return this->visible;
}
