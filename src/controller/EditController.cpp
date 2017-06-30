#include "../controller/EditController.hpp"
#include "../uitk/Layer.hpp"
#include "../view/BoardView.hpp"

/**
 * Constructor.
 * <p>
 * Creates a new level or loads an existing one depending on the initial state of the model.
 * @param model
 * @param display
 */
EditController::EditController(EditorModel *model, Display* display, Options* options) : BaseController(model, display) {
    BoardModel* board = getModel()->getBoard();
    // Create New Level for Edit
    if (options->getCreateNewLevel()) {
        board->setLevelNum((uint8_t) (getLevelManager()->getLevelCount() + 1));
        getLevelManager()->create();

        // Load Existing Level for Edit
    } else {
        board->setLevelNum(options->getStartingLevel());
        getLevelManager()->read();
    }

    // Add the Board to the Display
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = getDisplay()->getWidth();
    rect.h = 400;
    View* boardView = new BoardView(board, rect);
    boardView->show();
    getDisplay()->addView("board", boardView);
}

/**
 * Initialize the game elements for edit mode.
 */
void EditController::gameLoop() {
    // Limit to 25 frames per second
    uint32_t fps = 25;
    uint32_t maxDelay = 1000 / fps;

    while (getModel()->getState() != QUIT) {
        // Each iteration represents a frame

        // Begin preparing the frame
        uint32_t workStart = SDL_GetTicks();

        // Advance the animations because we're about to compose the frame
        getDisplay()->advanceAnimations();

        // Build and present the frame
        getDisplay()->draw();

        processInput();

        // Determine how much time we have left after doing work
        uint32_t workEnd = SDL_GetTicks();
        uint32_t workDuration = workEnd - workStart;
        int remainingTime = maxDelay - workDuration;

        // Sleep any remaining time so that we don't hog the CPU
        if (remainingTime > 0) {
            SDL_Delay((uint32_t) remainingTime);
        }
    }
}

/**
 * Handle events for edit mode.
 */
void EditController::processInput() {
    // Wait for an Event.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Key down.
        if (event.type == SDL_KEYDOWN) {
            // User Presses Q
            if (event.key.keysym.sym == SDLK_q) {
                getModel()->setState(QUIT);
                break;

            // Pass all other keys to the parent of the selected layer.  Or the
            // selected layer if it's a root layer.
            } else {
                Layer* selected = Layer::getSelectedLayer();
                if (selected != NULL) {
                    // Find the Event Handling Layer.  It'll either be the
                    // selected layer's parent or the selected layer (depending
                    // on whether the selected layer has no parent).
                    Layer* handler = selected->getParent();
                    if (handler == NULL) {
                        handler = selected;
                    }

                    handler->onKeyDown(event.key.keysym.sym);
                }
            }

            // Handle Quit Event.
        } else if (event.type == SDL_QUIT) {
            getModel()->setState(QUIT);
            break;

        }
    }
}

/**
 * Fetch the sub-classed model for this controller.
 * @return the model
 */
EditorModel *EditController::getModel() const {
    return (EditorModel*) BaseController::getModel();
}
