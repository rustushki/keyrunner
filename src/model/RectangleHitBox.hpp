#ifndef KEYRUNNER_RECTANGLE_HIT_BOX_HPP
#define KEYRUNNER_RECTANGLE_HIT_BOX_HPP

#include "../model/HitBox.hpp"

class RectangleHitBox : public HitBox {
public:
    bool intersects(HitBox* hitBox) const override;
    bool contains(Coordinate coordinate) const override;
    Coordinate getAnchor() const override;
    long getWidth() const;
    long getHeight() const;
    long getLeft() const;
    long getRight() const;
    long getTop() const;
    long getBottom() const;


private:
    uint32_t height;
    uint32_t width;
    Coordinate anchor;
};

#endif
