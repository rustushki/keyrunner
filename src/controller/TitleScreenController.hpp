#ifndef TITLE_SCREEN_CONTROLLER_HPP
#define TITLE_SCREEN_CONTROLLER_HPP

#include "DisplayController.hpp"
#include "../model/TitleScreenModel.hpp"

class TitleScreenController : public DisplayController {
public:
    TitleScreenController(TitleScreenModel* model, Display* display);
    ~TitleScreenController() override = default;

protected:
    TitleScreenModel* getModel() const override;
    void updateModel(long frameDuration) override;
    bool isStillExecuting() const override;

private:
    void createKeyImage();
    void createKeyRunnerText();
    void createPressEnterText();
    void createMainMenu();
    void createEditorLevelSelectorMenu();
};

#endif
