#ifndef KEYRUNNER_RECTANGLE_HIT_BOX_HPP
#define KEYRUNNER_RECTANGLE_HIT_BOX_HPP

#include "../model/HitBox.hpp"

class RectangleHitBox : public HitBox {
public:
    explicit RectangleHitBox(Coordinate anchor, long width, long height);

    bool intersects(HitBox* hitBox) const override;
    bool contains(Coordinate coordinate) const override;
    Coordinate getAnchor() const override;
    void setAnchor(Coordinate newAnchor) override;
    long getWidth() const;
    long getHeight() const;
    long getLeft() const;
    long getRight() const;
    long getTop() const;
    long getBottom() const;
    HitBoxType getType() const override;


private:
    long height;
    long width;
    Coordinate anchor;
};

#endif
