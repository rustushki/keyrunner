#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include <SDL_video.h>
#include "../controller/Controller.hpp"
#include "../controller/Display.hpp"
#include "../controller/Options.hpp"
#include "../model/Model.hpp"
#include "../model/State.hpp"
#include "../view/AnimationFactory.hpp"
#include "../model/GameModel.hpp"

class GameController : public Controller {
public:
    explicit GameController(GameModel* gameModel, Options options);
    virtual ~GameController();
    void gameLoop() override;

protected:
    GameModel* getModel() const final;
    void updateModel(long frameDuration) final;
    bool checkExitConditions() const final;

private:
    void buildDisplay();
    void initializeSdl();
    SDL_Window* createWindow();
    void sizeWindowAndShow(SDL_Window *window);
    SDL_Renderer* createRenderer(SDL_Window *window);

    GameModel* gameModel;
    Options options;
    Display *display;
    bool firstLoop;
};

#endif
