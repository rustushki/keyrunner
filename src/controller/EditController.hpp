#ifndef EDIT_CONTROLLER_HPP
#define EDIT_CONTROLLER_HPP

#include "../controller/BaseController.hpp"
#include "../controller/Options.hpp"

class EditController : public BaseController {
public:
    EditController(PlayModel* model, Display* display, Options* options);
    void gameLoop();
    void processInput();

};

#endif
