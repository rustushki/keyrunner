#ifndef HIT_BOX_HPP
#define HIT_BOX_HPP

#include <set>
#include "../model/Coordinate.hpp"
#include "../model/HitBoxType.hpp"

class HitBox {
public:
    virtual bool intersects(HitBox* hitBox) const  = 0;
    virtual bool contains(Coordinate coordinate) const = 0;
    virtual HitBoxType getType() const = 0;
    virtual Coordinate getAnchor() const = 0;
    virtual void setAnchor(Coordinate newAnchor) = 0;
    virtual std::set<TileCoordinate> getIntersectingTileCoordinates() const =  0;
};

#endif
