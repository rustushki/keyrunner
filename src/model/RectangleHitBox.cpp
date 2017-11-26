#include "../model/RectangleHitBox.hpp"

/**
 * Check to see if this hit box intersects with the provided.
 * @param hitBox
 * @return boolean
 */
bool RectangleHitBox::intersects(HitBox* hitBox) {
    bool intersects = false;
    if (hitBox->getType() == RECTANGLE) {
        auto that = dynamic_cast<RectangleHitBox*>(hitBox);
        if (that->getLeft() > this->getLeft() && that->getLeft() < this->getRight()) {
            if (that->getTop() > this->getTop() && that->getTop() < this->getBottom()) {
                intersects = true;
            }
        }
    }
    return intersects;
}

/**
 * Check to see if the provided coordinate falls within this hit box.
 * @param coordinate
 * @return boolean
 */
bool RectangleHitBox::contains(Coordinate coordinate) {
    bool contains = false;

    if (coordinate.getX() >= getLeft() && coordinate.getX() <= getRight()) {
        if (coordinate.getY() >= getTop() && coordinate.getY() <= getBottom()) {
            contains = true;
        }
    }

    return contains;
}

/**
 * Get the x dimension of the top left corner of the hit box.
 * @return uint32_t
 */
uint32_t RectangleHitBox::getX() const {
    return x;
}

/**
 * Get the y dimension of the top left corner of the hit box.
 * @return uint32_t
 */
uint32_t RectangleHitBox::getY() const {
    return y;
}

/**
 * Get the width of the hit box.
 * @return uint32_t
 */
uint32_t RectangleHitBox::getWidth() const {
    return width;
}

/**
 * Get the height of the hit box.
 * @return uint32_t
 */
uint32_t RectangleHitBox::getHeight() const {
    return height;
}

/**
 * Get the right edge of the hit box.
 * @return uint32_t
 */
uint32_t RectangleHitBox::getRight() const {
    return getX() + getWidth();
}

/**
 * Get the left edge of the hit box.
 * @return uint32_t
 */
uint32_t RectangleHitBox::getLeft() const {
    return getX();
}

/**
 * Get the top edge of the hit box.
 * @return uint32_t
 */
uint32_t RectangleHitBox::getTop() const {
    return getY();
}

/**
 * Get the bottom edge of the hit box.
 * @return uint32_t
 */
uint32_t RectangleHitBox::getBottom() const {
    return getY() + getHeight();
}
