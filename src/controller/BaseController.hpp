#ifndef BASE_CONTROLLER_HPP
#define BASE_CONTROLLER_HPP

#include "../controller/KeyRunner.hpp"
#include "../model/Direction.hpp"
#include "../controller/Controller.hpp"
#include "../model/LevelManager.hpp"

class BaseController : public Controller {
public:
    BaseController(BoardModel* model, Display* display);
    virtual void gameLoop() override = 0;

protected:
    virtual BoardModel* getModel() const override;
    virtual Display* getDisplay() const override;
    virtual void processInput() override = 0;

    LevelManager* getLevelManager() const;

private:
    BoardModel* model;
    Display* display;
    LevelManager* levelManager;
};

#endif