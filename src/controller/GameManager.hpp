#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <SDL_video.h>
#include "../controller/Controller.hpp"
#include "../controller/Display.hpp"
#include "../controller/Options.hpp"
#include "../model/Model.hpp"
#include "../model/State.hpp"
#include "../view/AnimationFactory.hpp"

class GameManager {
public:
    explicit GameManager(Options options);
    void loop();

private:
    void buildCurrentModel();
    void buildCurrentController(bool firstLoop);
    void buildDisplay();
    void initializeSdl();
    SDL_Window* createWindow();
    void sizeWindowAndShow(SDL_Window *window);
    SDL_Renderer* createRenderer(SDL_Window *window);

    Options options;
    Display *display;
    State previousState = {};
    Controller* previousController = nullptr;
    Model* previousModel = nullptr;
    State currentState = {};
    Controller* currentController = nullptr;
    Model* currentModel = nullptr;
};

#endif
