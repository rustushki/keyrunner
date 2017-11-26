#ifndef HIT_BOX_HPP
#define HIT_BOX_HPP

#include "../model/Coordinate.hpp"
#include "../model/HitBoxType.hpp"

class HitBox {
public:
    virtual bool intersects(HitBox* hitBox) = 0;
    virtual bool contains(Coordinate coordinate) = 0;
    virtual HitBoxType getType() = 0;
};

#endif
