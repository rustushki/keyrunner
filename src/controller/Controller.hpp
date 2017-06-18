#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../uitk/Display.hpp"

class Controller {
public:
    virtual void gameLoop() = 0;
    virtual void processInput() = 0;

protected:
    virtual PlayModel* getModel() = 0;
    virtual Display* getDisplay() = 0;

};


#endif
