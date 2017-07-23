#ifndef TITLE_SCREEN_CONTROLLER_HPP
#define TITLE_SCREEN_CONTROLLER_HPP

#include "BaseController.hpp"

class TitleScreenController : public BaseController {
public:
    TitleScreenController(BoardModel* model, Display* display);
    ~TitleScreenController() override = default;

protected:
    void processInput() override;
    void updateModel(long frameDuration) override;
    bool checkExitConditions() const override;

private:
    void createKeyImage();
    void createKeyRunnerText();
    void createPressEnterText();
    void createMainMenu();
};

#endif
