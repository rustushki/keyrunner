#ifndef EDIT_CONTROLLER_HPP
#define EDIT_CONTROLLER_HPP

#include "BaseController.hpp"

class EditController : public BaseController {
public:
    EditController(PlayModel* model, RootLayer* rootLayer, SDL_Window* window, SDL_Renderer* renderer);
    void gameLoop();
    void processInput();

};

#endif
