#ifndef PLAY_CONTROLLER_HPP
#define PLAY_CONTROLLER_HPP

#include "../controller/BoardController.hpp"
#include "../controller/Options.hpp"
#include "../model/PlayBoardModel.hpp"

class PlayController : public BoardController {
public:
    PlayController(PlayBoardModel* model, Display* display, Options* options);
    ~PlayController() override = default;

protected:
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
