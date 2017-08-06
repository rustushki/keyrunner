#ifndef TITLE_SCREEN_CONTROLLER_HPP
#define TITLE_SCREEN_CONTROLLER_HPP

#include "../controller/BaseController.hpp"
#include "../model/TitleScreenModel.hpp"

class TitleScreenController : public BaseController {
public:
    TitleScreenController(TitleScreenModel* model, Display* display);
    ~TitleScreenController() override = default;

protected:
    void updateModel(long frameDuration) override;
    bool checkExitConditions() const override;

private:
    void createKeyImage();
    void createKeyRunnerText();
    void createPressEnterText();
    void createMainMenu();
    void createEditorLevelSelectorMenu();
};

#endif
