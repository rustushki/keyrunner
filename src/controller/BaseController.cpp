
#include "../controller/BaseController.hpp"

/**
 * Constructor.
 * <p>
 * Sets the provided fields for subclasses.
 * @param model
 * @param display
 */
BaseController::BaseController(BoardModel* model, Display* display) {
    this->model = model;
    this->display = display;
    this->levelManager = new LevelManager(model);
}

/**
 * Destructor.
 */
BaseController::~BaseController() {}

/**
 * Return the root model that the Controller addresses.
 * @return BaseModel*
 */
BoardModel* BaseController::getModel() const {
    return model;
}

/**
 * Get the display where views are put onto the screen.
 * @return Display*
 */
Display *BaseController::getDisplay() const {
    return display;
}

/**
 * Get the Level Manager which can load levels into the board or save levels to persistent storage from the board.
 * @return LevelManager*
 */
LevelManager* BaseController::getLevelManager() const {
    return levelManager;
}

/**
 * Game loop. Update the model, prepare and display the frame, process input, and maintain a frame delay to meet a
 * certain FPS.
 */
void BaseController::gameLoop() {
    // Limit to 25 frames per second
    uint32_t fps = 25;
    uint32_t maxDelay = 1000 / fps;

    // Iterate frames until the exit conditions are met
    while (checkExitConditions()) {
        // Begin preparing the frame
        uint32_t workStart = SDL_GetTicks();

        // Advance the animations because we're about to compose the frame
        getDisplay()->advanceAnimations();

        // Build and present the frame
        getDisplay()->draw();

        // Collect input from the user
        processInput();

        // Determine how much time we have left after doing work
        uint32_t workEnd = SDL_GetTicks();
        uint32_t workDuration = workEnd - workStart;
        int remainingTime = maxDelay - workDuration;

        // Sleep any remaining time so that we don't hog the CPU
        if (remainingTime > 0) {
            SDL_Delay((uint32_t) remainingTime);
        }

        // Update the model, letting it know how much time it took to iterate the frame
        updateModel(SDL_GetTicks() - workStart);
    }
}
