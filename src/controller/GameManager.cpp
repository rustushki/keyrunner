#include "GameManager.hpp"
#include <sstream>
#include <SDL_ttf.h>
#include "../controller/PlayController.hpp"
#include "../controller/EditController.hpp"
#include "../controller/TitleScreenController.hpp"

extern AnimationFactory* animationFactory;

/**
 * Constructor.
 * @param options
 */
GameManager::GameManager(Options options) {
    this->options = options;
    this->buildDisplay();
}

void GameManager::loop() {
    // Start the Game Loop for the appropriate Controller
    currentState = options.getInitialState();

    bool firstLoop = true;

    while (currentState != QUIT && currentState != WIN && currentState != LOSE) {
        buildCurrentModel();
        buildCurrentController(firstLoop);

        // Set the initial state
        currentModel->setState(currentState);

        // Begin the game loop
        currentController->gameLoop();

        // Obtain the exit state
        State newState = currentModel->getState();

        // Free some memory
        delete currentModel;
        delete currentController;

        // Save the previous model, controller and state
        previousState = currentState;
        previousModel = currentModel;
        previousController = currentController;

        firstLoop = false;
        currentState = newState;
    }

    delete animationFactory;
}

/**
 * Initialize a number of graphical components.
 * <p>
 * This includes SDL itself, the window, the renderer, the global animation factory, and the display.
 */
void GameManager::buildDisplay() {
    // Initialize SDL
    initializeSdl();

    // Window
    SDL_Window* window = createWindow();
    sizeWindowAndShow(window);

    // Renderer
    SDL_Renderer* renderer = createRenderer(window);

    // Build the Display that will be used by the Controller
    this->display = new Display(window, renderer);

    // Build the global animation factory
    animationFactory = new AnimationFactory(renderer);
}

/**
 * Initializes SDL and SDL_ttf. Ensure SDL_Quit is called on exit.
 */
void GameManager::initializeSdl() {
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
SDL_Window* GameManager::createWindow() {
    std::stringstream title;
    title << "Key Runner v" << VERSION;
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
SDL_Renderer* GameManager::createRenderer(SDL_Window* window) {
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
void GameManager::sizeWindowAndShow(SDL_Window* window) {
    SDL_SetWindowSize(window, 625, 440);
    SDL_ShowWindow(window);
}


/**
 * Build the current model based on the current state.
 */
void GameManager::buildCurrentModel() {
    if (currentState == PLAY) {
        this->currentModel = new PlayBoardModel();
    } else if (currentState == EDIT) {
        this->currentModel = new EditorBoardModel();
    } else if (currentState == TITLE) {
        this->currentModel = new TitleScreenModel();
    } else {
        std::stringstream errorMessage;
        errorMessage << "could not build model from state (" << currentState << ")";
        throw std::invalid_argument(errorMessage.str());
    }
}

/**
 * Given the current and previous state and the current and previous model, build a controller.
 * <p>
 * Use the settings from the provided Options object on the first loop iteration.
 * @param firstLoop
 */
void GameManager::buildCurrentController(bool firstLoop) {
    Controller* controller = nullptr;

    if (currentState == PLAY) {
        auto startingLevel = static_cast<uint8_t>(1);

        // On first loop, pull the starting level from the command line options
        if (firstLoop) {
            startingLevel = options.getStartingLevel();
        }

        controller = new PlayController(dynamic_cast<PlayBoardModel*>(currentModel), display, startingLevel);

    } else if (currentState == EDIT) {
        auto editingLevel = static_cast<uint8_t>(0);

        // On first loop, pull the editing level from the command line options
        if (firstLoop) {
            // Create New Level for Edit
            if (options.getCreateNewLevel()) {
                editingLevel = 0;

                // Load Existing Level for Edit
            } else {
                editingLevel = options.getStartingLevel();
            }
        }

        controller = new EditController(dynamic_cast<EditorBoardModel*>(currentModel), display, editingLevel);

    } else if (currentState == TITLE) {
        controller = new TitleScreenController(dynamic_cast<TitleScreenModel*>(currentModel), display);
    }

    this->currentController = controller;
}
