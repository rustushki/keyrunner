#ifndef PLAY_CONTROLLER_HPP
#define PLAY_CONTROLLER_HPP

#include "../controller/BaseController.hpp"
#include "../controller/Options.hpp"
#include "../model/PlayBoardModel.hpp"

class PlayController : public BaseController {
public:
    PlayController(PlayBoardModel* model, Display* display, Options* options);
    ~PlayController() override;

protected:
    void processInput() override;
    PlayBoardModel* getModel() const override;
    void updateModel(long frameDuration) override;

private:
    void updateLevel(long elapsedDuration) const;
    void conveyPlayer() const;
    bool checkExitConditions() const override;

    View* createRectangle() const;
    View* createBoard() const;
    View* createTimer() const;
    View* createLevelLabel() const;
};

#endif
