#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../view/RootLayer.hpp"

class Controller {
public:
    virtual void gameLoop() = 0;
    virtual void processInput() = 0;

protected:
    virtual PlayModel* getModel() = 0;
    virtual SDL_Renderer* getRenderer() = 0;
    virtual SDL_Window* getWindow() = 0;
    virtual RootLayer* getRootLayer() = 0;
    virtual void updateDisplay() = 0;

};


#endif
