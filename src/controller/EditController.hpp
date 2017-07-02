#ifndef EDIT_CONTROLLER_HPP
#define EDIT_CONTROLLER_HPP

#include "../controller/BaseController.hpp"
#include "../controller/Options.hpp"
#include "../model/EditorModel.hpp"
#include "../uitk/ButtonView.hpp"

class EditController : public BaseController {
public:
    EditController(EditorModel* model, Display* display, Options* options);
    void gameLoop() override;

private:
    ButtonView* exitButton;
    void processInput() override;
    EditorModel* getModel() const override;

};

#endif
