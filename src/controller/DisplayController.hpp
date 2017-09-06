#ifndef BASE_CONTROLLER_HPP
#define BASE_CONTROLLER_HPP

#include "../controller/Controller.hpp"
#include "../controller/KeyRunner.hpp"
#include "../model/Direction.hpp"
#include "../model/LevelManager.hpp"

class DisplayController : public Controller {
public:
    DisplayController(BaseModel* model, Display* display);
    ~DisplayController() override = default;
    void execute() override;
    Display* getDisplay() const;

protected:
    BaseModel* getModel() const override;
    void processInput();
    void updateModel(long frameDuration) override = 0;
    bool isStillExecuting() const override = 0;

private:
    BaseModel* model;
    Display* display;
};

#endif
