#ifndef PLAY_CONTROLLER_HPP
#define PLAY_CONTROLLER_HPP

#include "../controller/BaseController.hpp"
#include "../controller/Options.hpp"

class PlayController : public BaseController {
public:
    PlayController(PlayModel* model, Display* display, Options* options);

    virtual void gameLoop();
    virtual void processInput();
};

#endif
