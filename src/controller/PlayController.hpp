#ifndef PLAY_CONTROLLER_HPP
#define PLAY_CONTROLLER_HPP

#include "../controller/BaseController.hpp"

class PlayController : public BaseController {
public:
    PlayController(PlayModel* model, Display* display);

    virtual void gameLoop();
    virtual void processInput();
};

#endif
