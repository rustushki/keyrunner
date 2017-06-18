#ifndef PLAY_CONTROLLER_HPP
#define PLAY_CONTROLLER_HPP


#include "BaseController.hpp"

class PlayController : public BaseController {
public:
    PlayController(PlayModel* model, RootLayer* rootLayer, SDL_Window* window, SDL_Renderer* renderer);

    void gameLoop();
    void processInput();

private:
    void moveDirection(Direction direction);
};


#endif
