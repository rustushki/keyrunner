#ifndef BASE_MODEL_HPP
#define BASE_MODEL_HPP

#include "../model/Model.hpp"
#include "../model/BoardModel.hpp"
#include "../model/State.hpp"
#include "../model/BaseModel.hpp"

class BaseModel : public Model {

public:
    BaseModel();
    ~BaseModel();
    BoardModel* getBoard();
    State getState() const;
    void setState(State state);

private:
    BoardModel* board;
    State state;
};

#endif
