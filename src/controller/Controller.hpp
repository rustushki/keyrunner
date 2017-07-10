#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../controller/Display.hpp"

class Controller {
public:
    virtual ~Controller() {}
    virtual void gameLoop() = 0;

protected:
    virtual Model* getModel() const = 0;
    virtual Display* getDisplay() const = 0;
    virtual void processInput() = 0;

};


#endif
