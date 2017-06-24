#ifndef BASE_CONTROLLER_HPP
#define BASE_CONTROLLER_HPP

#include "../controller/KeyRunner.hpp"
#include "../model/Direction.hpp"
#include "../model/PlayModel.hpp"
#include "../controller/Controller.hpp"

class BaseController : public Controller {
public:
    BaseController(PlayModel* model, Display* display);
    virtual void gameLoop() = 0;
    virtual void processInput() = 0;

protected:
    virtual PlayModel* getModel();
    virtual Display* getDisplay();

    void conveyPlayer();

private:
    PlayModel* model;
    Display* display;
};

#endif
