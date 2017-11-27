#ifndef KEYRUNNER_RECTANGLE_HIT_BOX_HPP
#define KEYRUNNER_RECTANGLE_HIT_BOX_HPP

#include "../model/HitBox.hpp"

class RectangleHitBox : public HitBox {
public:
    bool intersects(HitBox* hitBox) const override;
    bool contains(Coordinate coordinate) const override;
    Coordinate getAnchor() const override;
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    uint32_t getLeft() const;
    uint32_t getRight() const;
    uint32_t getTop() const;
    uint32_t getBottom() const;


private:
    uint32_t height;
    uint32_t width;
    Coordinate anchor;
};

#endif
