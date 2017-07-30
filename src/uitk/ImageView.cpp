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
    setHorizontalAlignment(HorizontalAlignment::LEFT);
    setVerticalAlignment(VerticalAlignment::BOTTOM);
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
    auto x = static_cast<uint16_t>(getX() + getHorizontalOffset());
    auto y = static_cast<uint16_t>(getY() + getVerticalOffset());
    animation->move(x, y);
    animation->draw(renderer);
}

/**
 * Set the internal horizontal alignment of the image.
 * @param alignment
 */
void ImageView::setHorizontalAlignment(HorizontalAlignment alignment) {
    this->horizontalAlignment = alignment;
}

/**
 * Set the internal vertical alignment of the image.
 * @param alignment
 */
void ImageView::setVerticalAlignment(VerticalAlignment alignment) {
    this->verticalAlignment = alignment;
}

/**
 * Get the internal horizontal alignment of the image.
 * @return HorizontalAlignment
 */
HorizontalAlignment ImageView::getHorizontalAlignment() const {
    return this->horizontalAlignment;
}

/**
 * Get the internal vertical alignment of the image.
 * @return VerticalAlignment
 */
VerticalAlignment ImageView::getVerticalAlignment() const {
    return this->verticalAlignment;
}

/**
 * Get the horizontal offset of the image within the ImageView.
 * <p>
 * This will be based off the horizontal alignment constant.
 * @return int
 */
int ImageView::getHorizontalOffset() const {
    int horizontalOffset = 0;
    if (getHorizontalAlignment() == HorizontalAlignment::CENTER) {
        horizontalOffset = (getWidth() - animation->getWidth()) / 2;
    } else if (getHorizontalAlignment() == HorizontalAlignment::RIGHT) {
        horizontalOffset = getWidth() - animation->getWidth();
    } else {
        horizontalOffset = 0;
    }

    return horizontalOffset;
}

/**
 * Get the vertical offset of the image within the ImageView.
 * <p>
 * This will be based off the vertical alignment constant.
 * @return int
 */
int ImageView::getVerticalOffset() const {
    int verticalOffset = 0;
    if (getVerticalAlignment() == VerticalAlignment ::CENTER) {
        verticalOffset = (getHeight() - animation->getWidth()) / 2;
    } else if (getVerticalAlignment() == VerticalAlignment::BOTTOM) {
        verticalOffset = getHeight() - animation->getWidth();
    } else {
        verticalOffset = 0;
    }

    return verticalOffset;
}
