#include <sstream>
#include <SDL_ttf.h>
#include "../controller/PlayController.hpp"
#include "../controller/EditController.hpp"
#include "../controller/GameController.hpp"
#include "../controller/TitleScreenController.hpp"

extern AnimationFactory* animationFactory;

/**
 * Constructor.
 * @param options
 */
GameController::GameController(GameModel* gameModel, int argc, char** argv) {
    // Populate the game model
    this->gameModel = gameModel;

    // Indicate that the game loop has not run once yet
    this->firstLoop = true;

    // Use the OptionController to populate the OptionModel with settings from command line arguments
    OptionModel* optionModel = getModel()->getOptionModel();
    OptionController optionController{optionModel, argc, argv};
    optionController.execute();

    // Set the initial state from the command line options
    getModel()->setState(optionModel->getInitialState());

    // Build the SDL display and window
    this->buildDisplay();
}

/**
 * Destructor.
 */
GameController::~GameController() {
    delete animationFactory;
}

/**
 * Updates the state of the game itself.
 * <p>
 * Fetches the current state. Uses that to determine which controller to build and model to fetch. Builds the
 * controller, passing in the appropriate model. Starts the controller's loop and waits for the loop to exit. When the
 * controller exits, it fetches the controller's exiting state from the controller's model. That state is the new game
 * state.
 * @param frameDuration
 */
void GameController::updateModel(long frameDuration) {
    Controller* controller = nullptr;

    State currentState = getModel()->getState();

    if (currentState == PLAY) {
        PlayBoardModel* model = getModel()->getPlayBoardModel();
        model->setState(PLAY);

        auto startingLevel = static_cast<uint8_t>(1);
        if (firstLoop) {
            startingLevel = getModel()->getOptionModel()->getStartingLevel();
        }

        controller = new PlayController(model, display, startingLevel);

    } else if (currentState == EDIT) {
        EditorBoardModel* model = getModel()->getEditorBoardModel();

        // Editing Level of 0 means create new level
        auto editingLevel = static_cast<uint8_t>(0);

        // On first loop, pull the editing level from the command line options
        if (firstLoop) {
            // Create New Level for Edit
            if (!getModel()->getOptionModel()->getCreateNewLevel()) {
                editingLevel = getModel()->getOptionModel()->getStartingLevel();
            }

        // Otherwise, pull it from the TitleScreenModel
        } else {
            if (!getModel()->getTitleScreenModel()->getCreateNewLevel()) {
                editingLevel = getModel()->getTitleScreenModel()->getEditorLevel();
            }
        }

        controller = new EditController(model, display, editingLevel);
    } else if (currentState == TITLE) {
        controller = new TitleScreenController(getModel()->getTitleScreenModel(), display);

    } else {
        throw std::logic_error("Invalid state detected");
    }

    controller->execute();
    firstLoop = false;

    State newState = controller->getModel()->getState();
    getModel()->setState(newState);

    delete controller;

}

/**
 * Confirms that the exit conditions are not yet met.
 * <p>
 * They are met if the current state is QUIT, WIN or LOSE.
 * @return boolean
 */
bool GameController::isStillExecuting() const {
    State currentState = getModel()->getState();
    return currentState != QUIT && currentState != WIN && currentState != LOSE;
}

/**
 * Initialize a number of graphical components.
 * <p>
 * This includes SDL itself, the window, the renderer, the global animation factory, and the display.
 */
void GameController::buildDisplay() {
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
void GameController::initializeSdl() {
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
SDL_Window* GameController::createWindow() {
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
SDL_Renderer* GameController::createRenderer(SDL_Window* window) {
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
void GameController::sizeWindowAndShow(SDL_Window* window) {
    SDL_SetWindowSize(window, 625, 440);
    SDL_ShowWindow(window);
}

/**
 * Gets the GameModel.
 * <p>
 * This model contains references to all models used by the entire game.
 * @return
 */
GameModel *GameController::getModel() const {
    return this->gameModel;
}

/**
 * While the exit conditions are not met, update the model.
 */
void GameController::execute() {
    while (isStillExecuting()) {
        updateModel(0);
    }
}