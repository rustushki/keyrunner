#ifndef MODEL_HPP
#define MODEL_HPP

#include "State.hpp"

class Model {
public:
    virtual State getState() const = 0;
    virtual void setState(State state) = 0;
};

#endif
