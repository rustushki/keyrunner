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
