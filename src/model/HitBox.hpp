#ifndef HIT_BOX_HPP
#define HIT_BOX_HPP

#include "../model/Coordinate.hpp"
#include "../model/HitBoxType.hpp"

class HitBox {
public:
    virtual bool intersects(HitBox* hitBox) const  = 0;
    virtual bool contains(Coordinate coordinate) const = 0;
    virtual HitBoxType getType() const = 0;
    virtual Coordinate getAnchor() const = 0;
};

#endif
