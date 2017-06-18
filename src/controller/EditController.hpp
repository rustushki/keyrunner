#ifndef EDIT_CONTROLLER_HPP
#define EDIT_CONTROLLER_HPP

#include "BaseController.hpp"

class EditController : public BaseController {
public:
    EditController(PlayModel* model, Display* display);
    void gameLoop();
    void processInput();

};

#endif
