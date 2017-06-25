#ifndef BASE_MODEL_HPP
#define BASE_MODEL_HPP

#include "Model.hpp"
#include "../model/Board.hpp"
#include "../model/State.hpp"
#include "../model/BaseModel.hpp"

class BaseModel : public Model {

public:
    BaseModel();
    ~BaseModel();
    Board* getBoard();
    State getState() const;
    void setState(State state);

private:
    Board* board;
    State state;
};

#endif
