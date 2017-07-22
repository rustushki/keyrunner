#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../controller/Display.hpp"

class Controller {
public:
    virtual ~Controller() = default;
    virtual void gameLoop() = 0;

protected:
    virtual Model* getModel() const = 0;
    virtual Display* getDisplay() const = 0;
    virtual void processInput() = 0;
    virtual void updateModel(long frameDuration) = 0;
    virtual bool checkExitConditions() const = 0;

};


#endif
