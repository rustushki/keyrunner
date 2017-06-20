#include <sstream>
#include <SDL_ttf.h>
#include "../controller/Options.hpp"
#include "../controller/PlayController.hpp"
#include "../controller/EditController.hpp"
#include "../view/AnimationFactory.hpp"
#include "../uitk/Animation.hpp"

// Objects that don't have a home yet
AnimationFactory* animationFactory;

/**
 * Initializes SDL and SDL_ttf. Ensure SDL_Quit is called on exit.
 */
void initializeSdl() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::stringstream errorMessage;
        errorMessage << "Couldn't initialize SDL: "<< SDL_GetError();
        throw std::runtime_error(errorMessage.str());
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::stringstream errorMessage;
        errorMessage << "Error initializing SDL_ttf: " << TTF_GetError();
        throw std::runtime_error(errorMessage.str());
    }

    // Ensure SDL_Quit is called when the program exits
    atexit(SDL_Quit);
}

/**
 * Create the SDL_Window* for the game, setting it's title.
 * @return SDL_Window*
 */
SDL_Window* createWindow() {
    std::stringstream title;
    title << "Key Runner r" << VERSION;
    SDL_Window* window = SDL_CreateWindow(title.str().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0,
          SDL_WINDOW_HIDDEN);

    // Throw an exception if the Window was not created
    if (window == nullptr) {
        std::stringstream errorMessage;
        errorMessage << "Couldn't create window: "<< SDL_GetError();
        throw std::runtime_error(errorMessage.str());
    }

    return window;
}

/**
 * Create a renderer for the SDL_Window.
 * @param window
 * @return SDL_Renderer*
 */
SDL_Renderer* createRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
        std::stringstream errorMessage;
        errorMessage << "Couldn't create renderer: "<< SDL_GetError();
        throw std::runtime_error(errorMessage.str());
    }

    return renderer;
}

/**
 * Size and show the window based on the RootLayer's dimensions.
 * @param window
 * @param rootLayer
 */
void sizeWindowAndShow(SDL_Window* window) {
    SDL_SetWindowSize(window, 625, 440);
    SDL_ShowWindow(window);
}

/**
 * Entry point into KeyRunner.
 * @param argc count of arguments
 * @param argv array of string arguments
 * @return exit code
 */
int main(int argc, char** argv) {
    // Parse the command line options
    Options::parse(argc, argv);

    // Initialize SDL
    initializeSdl();
    SDL_Window* window = createWindow();
    SDL_Renderer* renderer = createRenderer(window);

    // Create these view-related globals which don't have better homes
    animationFactory = new AnimationFactory(renderer);

    // Create the Model
    PlayModel* model = PlayModel::GetInstance();
    model->setState(Options::getInitialState());

    sizeWindowAndShow(window);

    // Build the Display that will be used by the Controller
    Display* display = new Display(window, renderer);

    // Start the Game Loop for the appropriate Controller
    Controller* controller = nullptr;
    if (model->getState() == PLAY) {
        controller = new PlayController(model, display);
    } else {
        controller = new EditController(model, display);
    }
    controller->gameLoop();

    // Free some memory
    delete animationFactory;
    delete controller;
    delete display;

    return 0;
}
