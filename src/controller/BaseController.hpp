#ifndef BASE_CONTROLLER_HPP
#define BASE_CONTROLLER_HPP

#include "../controller/KeyRunner.hpp"
#include "../model/Direction.hpp"
#include "../model/PlayModel.hpp"
#include "Controller.hpp"

class RootLayer;

class BaseController : public Controller {
public:
    BaseController(PlayModel* model, RootLayer* rootLayer, SDL_Window* window, SDL_Renderer* renderer);
    virtual void gameLoop() = 0;
    virtual void processInput() = 0;

protected:
    virtual PlayModel* getModel();
    virtual SDL_Renderer* getRenderer();
    virtual SDL_Window* getWindow();
    virtual RootLayer* getRootLayer();
    virtual void updateDisplay();

    void conveyPlayer();

private:
    PlayModel*    model;
    SDL_Window*   window;
    SDL_Renderer* renderer;
    RootLayer*    rootLayer;
};

#endif
