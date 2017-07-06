#include <sstream>
#include "../controller/PlayController.hpp"
#include "../view/BoardView.hpp"
#include "../uitk/RectangleView.hpp"
#include "../uitk/LabelView.hpp"
#include "../view/TimerView.hpp"
#include "../view/LevelNumberView.hpp"

/**
 * Constructor.
 * <p>
 * Sets the initial value of the time clock and the starting level based on the provided command line options. Creates
 * all Views used by the PlayController.
 * @param model
 * @param rootLayer
 * @param window
 * @param renderer
 */
PlayController::PlayController(PlayModel *model, Display* display, Options* options) : BaseController(model, display) {
    getModel()->setTimeClock(50000);
    getModel()->getBoard()->setLevelNum(options->getStartingLevel());

    // Add the Board to the Display
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = getDisplay()->getWidth();
    rect.h = 400;
    View* board = new BoardView(getModel()->getBoard(), rect);
    board->show();
    getDisplay()->addView("board", board);

    // Add the Info Bar to the display
    rect.x = 0;
    rect.h = 40;
    rect.y = display->getHeight() - rect.h;
    rect.w = display->getWidth();
    RectangleView* playInfoBar = new RectangleView(nullptr, rect);
    playInfoBar->setColor(0);
    playInfoBar->show();
    getDisplay()->addView("play_info_bar", playInfoBar);

    // Add the Timer to the display
    rect.w = 100;
    rect.x = display->getWidth() - rect.w;
    rect.h = 40;
    rect.y = display->getHeight() - rect.h;
    TimerView* timer = new TimerView(getModel(), rect);
    timer->setFontPath(FONT_PATH);
    timer->setFontSize(25);
    timer->setColor(0x000000);
    timer->setTextColor(0xEEEEEE);
    timer->show();
    getDisplay()->addView("timer", timer);

    // Add the Level Number to the display
    rect.w = 100;
    rect.x = 20;
    rect.h = 40;
    rect.y = display->getHeight() - rect.h;
    LevelNumberView* levelNumber = new LevelNumberView(getModel()->getBoard(), rect);
    levelNumber->setFontPath(FONT_PATH);
    levelNumber->setFontSize(25);
    levelNumber->setColor(0x000000);
    levelNumber->setTextColor(0xEEEEEE);
    levelNumber->show();
    getDisplay()->addView("level_number", levelNumber);
}

/**
 * Initializes the elements required for the 'play' mode.
 *
 * This includes initializing SDL, and kicking off the game loop.
 */
void PlayController::gameLoop() {
    // Limit to 25 frames per second
    uint32_t fps = 25;
    uint32_t maxDelay = 1000 / fps;

    // Read in the starting level
    getLevelManager()->read();

    // Begin the game loop and continue while not in the quit state
    while (getModel()->getState() == PLAY) {
        // Each iteration represents a frame

        // Begin preparing the frame
        uint32_t workStart = SDL_GetTicks();

        // Advance the animations because we're about to compose the frame
        getDisplay()->advanceAnimations();

        // Build and present the frame
        getDisplay()->draw();

        // Move the player along the conveyor belts (if applicable)
        conveyPlayer();

        // Handle supported system events
        processInput();

        // Determine how much time we have left after doing work
        uint32_t workEnd = SDL_GetTicks();
        uint32_t workDuration = workEnd - workStart;
        int remainingTime = maxDelay - workDuration;

        // Sleep any remaining time so that we don't hog the CPU
        if (remainingTime > 0) {
            SDL_Delay((uint32_t) remainingTime);
        }

        updateLevel(SDL_GetTicks() - workStart);
    }
}

void PlayController::updateLevel(long elapsedDuration) const {
    BoardModel* board = getModel()->getBoard();

    // If the level is complete,
    if (board->isComplete()) {
        // Check for winning conditions. If the next level is beyond the maximum level; win!
        uint8_t nextLevel = board->getLevelNum() + (uint8_t) +1;
        if (nextLevel > getLevelManager()->getLevelCount()) {
            getModel()->setState(WIN);

        // Otherwise, go to next level; adding some extra time to the clock
        } else {
            board->setLevelNum(nextLevel);
            getLevelManager()->read();
            getModel()->incrementTimeClock(6000);
        }

    // Check for losing conditions. If the clock runs down to 0; game over
    } else {
        getModel()->decrementTimeClock(elapsedDuration);
        if (getModel()->getTimeClock() <= 0) {
            getModel()->setState(LOSE);
        }
    }
}

/**
 * Process user input for the current frame.
 * <p>
 * Poll for events in a loop.  If the event is a directional key, process it as movement only once for the frame;
 * discarding all other directional key presses.  Also handles Q key to quit, and the SDL_QUIT event type.
 */
void PlayController::processInput() {
    SDL_Event event;
    bool alreadyMoved = false;
	BoardModel* board = getModel()->getBoard();
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            // User Presses Q
            if (event.key.keysym.sym == SDLK_q) {
                getModel()->setState(QUIT);
                break;
            }

            // Limit movement to once per frame
            if (!alreadyMoved) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    board->movePlayerInDirection(DIRECTION_DOWN);

                } else if (event.key.keysym.sym == SDLK_UP) {
                    board->movePlayerInDirection(DIRECTION_UP);

                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    board->movePlayerInDirection(DIRECTION_LEFT);

                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    board->movePlayerInDirection(DIRECTION_RIGHT);

                }
                alreadyMoved = true;

                // If the prior movement causes the level to be complete,
                // signal that the new level may be loaded.
                if (board->isComplete()){
                    return;
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
PlayModel *PlayController::getModel() const {
    return (PlayModel*) BaseController::getModel();
}
