#ifndef BASE_CONTROLLER_HPP
#define BASE_CONTROLLER_HPP

#include "../controller/KeyRunner.hpp"
#include "../model/Direction.hpp"
#include "../controller/Controller.hpp"
#include "../model/LevelManager.hpp"

class BaseController : public Controller {
public:
    BaseController(BaseModel* model, Display* display);
    ~BaseController() override;
    void gameLoop() override;

protected:
    BaseModel* getModel() const override;
    Display* getDisplay() const override;
    void processInput() override = 0;
    void updateModel(long frameDuration) override = 0;
    bool checkExitConditions() const override = 0;

private:
    BaseModel* model;
    Display* display;
};

#endif
