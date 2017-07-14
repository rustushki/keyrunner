#ifndef TITLE_SCREEN_CONTROLLER_HPP
#define TITLE_SCREEN_CONTROLLER_HPP

#include "BaseController.hpp"

class TitleScreenController : public BaseController {
public:
    TitleScreenController(BoardModel* model, Display* display);
    virtual ~TitleScreenController() override;

protected:
    virtual void processInput() override;
    virtual void updateModel(long frameDuration) override;
    virtual bool checkExitConditions() const override;

private:
    void createKeyImage();
};

#endif