#ifndef KEYRUNNER_HIT_BOX_HPP
#define KEYRUNNER_HIT_BOX_HPP

class HitBox {
    virtual bool intersects(HitBox* hitBox) = 0;
};

#endif
