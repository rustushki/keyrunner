#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../controller/KeyRunner.hpp"
#include "../model/Direction.hpp"
#include "../model/PlayModel.hpp"

class Animation;
class RootLayer;
class AnimationFactory;

extern Animation* KeyAnimation;
extern Animation* PlayerAnimation;
extern AnimationFactory* animationFactory;

class Controller {
public:
    ~Controller();

    void play();
    void edit();

private:

    // Helpers
    bool init();
    void moveDirection(Direction direction);
    void editHandleEvents();
    void processInput();
    void conveyPlayer();
    void updateDisplay();

    SDL_Window*   window;
    SDL_Renderer* renderer;
    RootLayer*    rootLayer;
    PlayModel*    playModel;
};

#endif
