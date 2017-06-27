#ifndef BASE_CONTROLLER_HPP
#define BASE_CONTROLLER_HPP

#include "../controller/KeyRunner.hpp"
#include "../model/Direction.hpp"
#include "../controller/Controller.hpp"
#include "../model/BaseModel.hpp"
#include "../model/LevelManager.hpp"

class BaseController : public Controller {
public:
    BaseController(BaseModel* model, Display* display);
    virtual void gameLoop() = 0;
    virtual void processInput() = 0;

protected:
    virtual BaseModel* getModel() const;
    virtual Display* getDisplay();

    void conveyPlayer();
    LevelManager* getLevelManager() const;

private:
    BaseModel* model;
    Display* display;
    LevelManager* levelManager;
};

#endif