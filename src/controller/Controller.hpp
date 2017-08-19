#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../controller/Display.hpp"

class Controller {
public:
    virtual ~Controller() = default;
    virtual void gameLoop() = 0;
    virtual Model* getModel() const = 0;

protected:
    virtual void updateModel(long frameDuration) = 0;
    virtual bool isStillExecuting() const = 0;
};


#endif
