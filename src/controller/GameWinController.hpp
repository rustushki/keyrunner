#ifndef GAME_WIN_CONTROLLER_HPP
#define GAME_WIN_CONTROLLER_HPP

#include "../controller/DisplayController.hpp"
#include "../model/GameWinModel.hpp"

class GameWinController : public DisplayController {
public:
    GameWinController(BaseModel* model, Display* display);

protected:
    GameWinModel* getModel() const final;
    void updateModel(long frameDuration) override;
    bool isStillExecuting() const override;
    View* createMenu() const;
    View* createYouWinLabel() const;
};

#endif
