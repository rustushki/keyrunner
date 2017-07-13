#ifndef PLAY_CONTROLLER_HPP
#define PLAY_CONTROLLER_HPP

#include "../controller/BaseController.hpp"
#include "../controller/Options.hpp"
#include "../model/PlayBoardModel.hpp"

class PlayController : public BaseController {
public:
    PlayController(PlayBoardModel* model, Display* display, Options* options);
    virtual ~PlayController() override;

protected:
    virtual void processInput() override;
    PlayBoardModel* getModel() const override;
    virtual void updateModel(long frameDuration);

private:
    void updateLevel(long elapsedDuration) const;
    void conveyPlayer() const;
    virtual bool checkExitConditions() const override;

    View* createRectangle() const;
    View* createBoard() const;
    View* createTimer() const;
    View* createLevelLabel() const;
};

#endif
