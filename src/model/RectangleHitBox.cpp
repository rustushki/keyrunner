#include "../model/RectangleHitBox.hpp"
#include "../model/TileCoordinate.hpp"

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
        if (that->getLeft() < this->getRight() && that->getRight() > this->getLeft()) {
            if (that->getTop() < this->getBottom() && that->getBottom() > this->getTop()) {
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
 * @return long
 */
long RectangleHitBox::getHeight() const {
    return height;
}

/**
 * Get the right edge of the hit box.
 * @return long
 */
long RectangleHitBox::getRight() const {
    return getAnchor().getX() + getWidth();
}

/**
 * Get the left edge of the hit box.
 * @return long
 */
long RectangleHitBox::getLeft() const {
    return getAnchor().getX();
}

/**
 * Get the top edge of the hit box.
 * @return long
 */
long RectangleHitBox::getTop() const {
    return getAnchor().getY();
}

/**
 * Get the bottom edge of the hit box.
 * @return long
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

/**
 * Return the set of TileCoordinates which intersect with this hit box.
 * @return std::set<TileCoordinate>
 */
std::set<TileCoordinate> RectangleHitBox::getIntersectingTileCoordinates() const {
    std::set<TileCoordinate> intersectingTileCoordinates;
    TileCoordinate topLeft(getAnchor());
    TileCoordinate bottomRight(Coordinate(getRight(), getBottom()));

    for (long x = topLeft.getX(); x <= bottomRight.getX(); x++) {
        for (long y = topLeft.getY(); y <= bottomRight.getY(); y++) {
            intersectingTileCoordinates.insert(TileCoordinate(x, y));
        }
    }

    return intersectingTileCoordinates;
}

