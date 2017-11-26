#ifndef KEYRUNNER_RECTANGLE_HIT_BOX_HPP
#define KEYRUNNER_RECTANGLE_HIT_BOX_HPP

#include "../model/HitBox.hpp"

class RectangleHitBox : public HitBox {
public:
    bool intersects(HitBox* hitBox) override;
    bool contains(Coordinate coordinate) override;
    uint32_t getX() const;
    uint32_t getY() const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    uint32_t getLeft() const;
    uint32_t getRight() const;
    uint32_t getTop() const;
    uint32_t getBottom() const;


private:
    uint32_t height;
    uint32_t width;
    uint32_t y;
    uint32_t x;
};

#endif
