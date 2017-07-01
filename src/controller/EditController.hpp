#ifndef EDIT_CONTROLLER_HPP
#define EDIT_CONTROLLER_HPP

#include "../controller/BaseController.hpp"
#include "../controller/Options.hpp"
#include "../model/EditorModel.hpp"
#include "../uitk/ButtonView.hpp"

class EditController : public BaseController {
public:
    EditController(EditorModel* model, Display* display, Options* options);
    void gameLoop();
    void processInput();
    EditorModel* getModel() const;

private:
    ButtonView* exitButton;

};

#endif
