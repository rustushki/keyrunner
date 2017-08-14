#ifndef OPTION_MODEL_HPP
#define OPTION_MODEL_HPP

#include "../model/Model.hpp"
#include "../model/State.hpp"

class OptionModel : public Model {
public:
    OptionModel();

    State getState() const final;
    void setState(State newState) override;
    uint8_t getStartingLevel();
    State getInitialState();
    bool getCreateNewLevel();
    void setStartingLevel(uint8_t startingLevel);
    void setInitialState(State state);
    void setCreateNewLevel(bool createNewLevel);

private:
    uint8_t startingLevel;
    State initialState;
    bool createNewLevel;
};

#endif
