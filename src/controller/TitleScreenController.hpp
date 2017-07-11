#ifndef TITLE_SCREEN_CONTROLLER_HPP
#define TITLE_SCREEN_CONTROLLER_HPP

#include "BaseController.hpp"

class TitleScreenController : public BaseController {
public:
    TitleScreenController(BoardModel* model, Display* display);
    virtual ~TitleScreenController() override;
    virtual void gameLoop() override;

protected:
    virtual void processInput() override;
};

#endif