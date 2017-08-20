
#include "DisplayController.hpp"

/**
 * Constructor.
 * <p>
 * Sets the provided fields for subclasses.
 * @param model
 * @param display
 */
DisplayController::DisplayController(BaseModel* model, Display* display) {
    this->model = model;
    this->display = display;
    this->display->reset();
}

/**
 * Return the root model that the Controller addresses.
 * @return BaseModel*
 */
BaseModel* DisplayController::getModel() const {
    return model;
}

/**
 * Get the display where views are put onto the screen.
 * @return Display*
 */
Display *DisplayController::getDisplay() const {
    return display;
}

/**
 * Game loop. Update the model, prepare and display the frame, process input, and maintain a frame delay to meet a
 * certain FPS.
 */
void DisplayController::execute() {
    // Limit to 25 frames per second
    uint32_t fps = 25;
    uint32_t maxDelay = 1000 / fps;

    // Iterate frames until the exit conditions are met
    while (isStillExecuting()) {
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

/**
 * Polls for events and handles them.
 * <p>
 * Generally, handling works like this:
 * <ul>
 * <li>Key events are delegated to the focused view</li>
 * <li>Mouse events are delegated to the first view under the mouse</li>
 * <li>Quit events always cause the controller to exit</li>
 * <ul>
 */
void DisplayController::processInput() {
    SDL_Event event = {};
    while (SDL_PollEvent(&event) == 1 && getModel()->getState() != QUIT) {

        // Key Down Events are passed to focused view
        if (event.type == SDL_KEYDOWN) {
            std::string focusedViewName = getDisplay()->getFocus();
            View* focusedView = getDisplay()->getViewByName(focusedViewName);
            if (focusedView != nullptr) {
                focusedView->onKeyDown(event);
            }

        // Key Up Events are passed to focused view
        } else if (event.type == SDL_KEYUP) {
            std::string focusedViewName = getDisplay()->getFocus();
            View* focusedView = getDisplay()->getViewByName(focusedViewName);
            if (focusedView != nullptr) {
                focusedView->onKeyUp(event);
            }

        // Quit Events will cause the game to exit
        } else if (event.type == SDL_QUIT) {
            getModel()->setState(QUIT);


        // Delegate Mouse Up, Down, and Hover to the views as they apply (disregarding focus)
        } else if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN ||
                   event.type == SDL_MOUSEMOTION) {
            auto x = (uint32_t) event.button.x;
            auto y = (uint32_t) event.button.y;
            View *view = getDisplay()->getViewUnderCoordinate(x, y);
            if (view != nullptr) {
                if (event.type == SDL_MOUSEBUTTONUP) {
                    view->onMouseUp(event);
                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    view->onMouseDown(event);
                } else if (event.type == SDL_MOUSEMOTION) {
                    view->onMouseHover(event);
                }
            }
        }
    }
}
