#include "ImageView.hpp"
#include "../view/AnimationFactory.hpp"

extern AnimationFactory* animationFactory;

/**
 * Constructor.
 * <p>
 * Builds an animation from the animation type so that it can be drawn later.
 * @param model
 * @param rect
 * @param animationType
 */
ImageView::ImageView(Model *model, const SDL_Rect &rect, AnimationType animationType) : RectangleView(model, rect) {
    animation = animationFactory->build(animationType);
}

/**
 * Destructor.
 * <p>
 * Frees the built animation.
 */
ImageView::~ImageView() {
    delete animation;
}

/**
 * Draw the image onto the renderer.
 * @param renderer
 */
void ImageView::draw(SDL_Renderer* renderer) {
    RectangleView::draw(renderer);
    animation->move(getX(), getY());
    animation->draw(renderer);
}
