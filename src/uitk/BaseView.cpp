#include "../uitk/BaseView.hpp"

/**
 * Constructor.
 * <p>
 * Store the model and rectangle region of this View.
 * @param model where to find data to display
 * @param rect where to display on the screen
 */
BaseView::BaseView(Model* model, SDL_Rect rect) {
    this->model = model;
    this->rect = rect;
    this->visible = false;
}

/**
 * Destructor.
 * <p>
 * Free all stored animations.
 */
BaseView::~BaseView() {
    const std::vector<Animation*> animations = getAnimations();
    for (Animation* animation : animations) {
        delete animation;
    }
}

/**
 * Get the model that contains the information to display.
 * @return PlayModel*
 */
Model* BaseView::getModel() const {
    return this->model;
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

/**
 * Get the vector of Animations which need to be displayed in this View.
 * @return std::vector<Animation*>
 */
const std::vector<Animation*> BaseView::getAnimations() const {
    return animations;
}

/**
 * Add and play an Animation to the vector of those that need to be animated in this View.
 * @param animation
 */
void BaseView::addAnimation(Animation* animation) {
    animation->play();
    animations.push_back(animation);
}

/**
 * Determine if the view is currently in a state of being pressed.
 * @return boolean
 */
bool BaseView::isPressed() const {
    return false;
}

/**
 * Invoke the callback function which handles the mouse press.
 */
void BaseView::onClick() const {
    this->onClickCallBack();
}

/**
 * Set the callback function which will handle the mouse press.
 * @param newCallBack
 */
void BaseView::setOnClickCallback(const std::function<void()> &newCallBack) {
    this->onClickCallBack = newCallBack;
}

/**
 * Set the new height of the View.
 * @param newHeight
 */
void BaseView::setHeight(uint16_t newHeight) {
    rect.h = newHeight;
}

/**
 * Set the new width of the View.
 * @param newHeight
 */
void BaseView::setWidth(uint16_t newWidth) {
    rect.w = newWidth;
}

/**
 * Set the new x coordinate of the View.
 * @param newHeight
 */
void BaseView::setX(uint16_t newX) {
    rect.x = newX;
}

/**
 * Set the new y coordinate of the View.
 * @param newHeight
 */
void BaseView::setY(uint16_t newY) {
    rect.x = newY;
}

/**
 * Get the height of the view.
 * @return uint16_t
 */
uint16_t BaseView::getHeight() const {
    return (uint16_t) rect.h;
}

/**
 * Get the width of the view.
 * @return uint16_t
 */
uint16_t BaseView::getWidth() const {
    return (uint16_t) rect.w;
}

/**
 * Get the x coordinate of the view.
 * @return uint16_t
 */
uint16_t BaseView::getX() const {
    return (uint16_t) rect.x;
}

/**
 * Get the y coordinate of the view.
 * @return uint16_t
 */
uint16_t BaseView::getY() const {
    return (uint16_t) rect.y;
}

/**
 * Determine is the provided x and y coordinate pair falls within the rectangular region of this view.
 * @param x
 * @param y
 * @return boolean
 */
bool BaseView::containsPoint(uint32_t x, uint32_t y) const {
    bool containsPoint = false;
    if (x >= getX() && x < getX() + getWidth()) {
        if (y >= getY() && y < getY() + getHeight()) {
            containsPoint = true;
        }
    }
    return containsPoint;
}
