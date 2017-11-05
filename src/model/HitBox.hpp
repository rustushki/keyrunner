#ifndef HIT_BOX_HPP
#define HIT_BOX_HPP

class HitBox {
    virtual bool intersects(HitBox* hitBox) = 0;
};

#endif
