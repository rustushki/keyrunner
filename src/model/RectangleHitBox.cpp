#include "../model/RectangleHitBox.hpp"

/**
 * Constructor.
 * @param anchor
 * @param width
 * @param height
 */
RectangleHitBox::RectangleHitBox(Coordinate anchor, long width, long height) : anchor(anchor), width(width),
        height(height) {}

/**
 * Check to see if this hit box intersects with the provided.
 * @param hitBox
 * @return boolean
 */
bool RectangleHitBox::intersects(HitBox* hitBox) const {
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
bool RectangleHitBox::contains(Coordinate coordinate) const {
    bool contains = false;

    if (coordinate.getX() >= getLeft() && coordinate.getX() <= getRight()) {
        if (coordinate.getY() >= getTop() && coordinate.getY() <= getBottom()) {
            contains = true;
        }
    }

    return contains;
}

/**
 * Get the anchor point of the hit box.
 * @return Coordinate
 */
Coordinate RectangleHitBox::getAnchor() const {
    return anchor;
}

/**
 * Get the width of the hit box.
 * @return long
 */
long RectangleHitBox::getWidth() const {
    return width;
}

/**
 * Get the height of the hit box.
 * @return uint32_t
 */
long RectangleHitBox::getHeight() const {
    return height;
}

/**
 * Get the right edge of the hit box.
 * @return uint32_t
 */
long RectangleHitBox::getRight() const {
    return getAnchor().getX() + getWidth();
}

/**
 * Get the left edge of the hit box.
 * @return uint32_t
 */
long RectangleHitBox::getLeft() const {
    return getAnchor().getX();
}

/**
 * Get the top edge of the hit box.
 * @return uint32_t
 */
long RectangleHitBox::getTop() const {
    return getAnchor().getY();
}

/**
 * Get the bottom edge of the hit box.
 * @return uint32_t
 */
long RectangleHitBox::getBottom() const {
    return getAnchor().getY() + getHeight();
}

/**
 * Set the point at which this hit box is anchored.
 * <p>
 * For a rectangle hit box, the anchor is the upper left corner.
 * @param newAnchor
 */
void RectangleHitBox::setAnchor(Coordinate newAnchor) {
    this->anchor = newAnchor;
}

/**
 * Return the type of hit box.
 * <p>
 * Always returns RECTANGLE.
 * @return HitBoxType
 */
HitBoxType RectangleHitBox::getType() const {
    return RECTANGLE;
}

