#ifndef BASE_MODEL_HPP
#define BASE_MODEL_HPP

#include "../model/Model.hpp"

class BaseModel : public Model {
public:
    State getState() const override;
    void setState(State state) override;

private:
    State state;
};

#endif
