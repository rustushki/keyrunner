#ifndef GAME_OVER_CONTROLLER_HPP
#define GAME_OVER_CONTROLLER_HPP

#include "../controller/DisplayController.hpp"
#include "../model/GameOverModel.hpp"

class GameOverController : public DisplayController {
public:
    GameOverController(GameOverModel* model, Display* display);


protected:
    GameOverModel* getModel() const final;
    void updateModel(long frameDuration) override;
    bool isStillExecuting() const override;

private:
    View* createMainMenu();
    View* createGameOverMenu();
};

#endif
