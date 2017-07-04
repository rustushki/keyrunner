#ifndef PLAY_CONTROLLER_HPP
#define PLAY_CONTROLLER_HPP

#include "../controller/BaseController.hpp"
#include "../controller/Options.hpp"
#include "../model/PlayModel.hpp"

class PlayController : public BaseController {
public:
    PlayController(PlayModel* model, Display* display, Options* options);
    virtual void gameLoop() override;

protected:
    virtual void processInput() override;
    PlayModel* getModel() const override;

private:
    void updateLevel(long elapsedDuration) const;
};

#endif
