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
 * @param window
 * @param renderer
 */
PlayController::PlayController(PlayBoardModel *model, Display* display, Options* options) : BaseController(model, display) {
    // Initialize the model
    getModel()->setTimeClock(50000);
    getModel()->setLevelNum(options->getStartingLevel());

    // Read in the starting level
    getLevelManager()->read();

    // Create each of the views for this controller
    View* board = createBoard();
    getDisplay()->addView("board", board);

    View* rectangle = createRectangle();
    getDisplay()->addView("rectangle", rectangle);

    View* timer = createTimer();
    getDisplay()->addView("timer", timer);

    View* levelLabel = createLevelLabel();
    getDisplay()->addView("level_number", levelLabel);
}

/**
 * Destructor.
 */
PlayController::~PlayController() {}

void PlayController::updateLevel(long elapsedDuration) const {
    PlayBoardModel* board = getModel();

    // If the level is complete,
    if (board->isComplete()) {
        // Check for winning conditions. If the next level is beyond the maximum level; win!
        uint8_t nextLevel = board->getLevelNum() + (uint8_t) +1;
        if (nextLevel > getLevelManager()->getLevelCount()) {
            getModel()->setState(WIN);

        // Otherwise, go to next level; adding some extra time to the clock
        } else {
            board->setPlayerHasKey(false);
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
	PlayBoardModel* board = getModel();
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
PlayBoardModel *PlayController::getModel() const {
    return (PlayBoardModel*) BaseController::getModel();
}

/**
 * Create a level label for the informational bar.
 */
View* PlayController::createLevelLabel() const {
    SDL_Rect rect;
    rect.w = 100;
    rect.x = 20;
    rect.h = 40;
    rect.y = getDisplay()->getHeight() - rect.h;
    LevelNumberView* levelNumber = new LevelNumberView(getModel(), rect);
    levelNumber->setFontPath(FONT_PATH);
    levelNumber->setFontSize(25);
    levelNumber->setColor(0x000000);
    levelNumber->setTextColor(0xEEEEEE);
    levelNumber->show();
    return levelNumber;
}

/**
 * Create a timer for the informational bar.
 */
View* PlayController::createTimer() const {
    SDL_Rect rect;
    rect.w = 100;
    rect.x = getDisplay()->getWidth() - rect.w;
    rect.h = 40;
    rect.y = getDisplay()->getHeight() - rect.h;
    TimerView* timer = new TimerView(getModel(), rect);
    timer->setFontPath(FONT_PATH);
    timer->setFontSize(25);
    timer->setColor(0x000000);
    timer->setTextColor(0xEEEEEE);
    timer->show();
    return timer;
}

/**
 * Create a black rectangle for the informational bar.
 */
View* PlayController::createRectangle() const {
    SDL_Rect rect;
    rect.x = 0;
    rect.h = 40;
    rect.y = getDisplay()->getHeight() - rect.h;
    rect.w = getDisplay()->getWidth();
    RectangleView* rectangle = new RectangleView(nullptr, rect);
    rectangle->setColor(0);
    rectangle->show();
    return rectangle;
}

/**
 * Create the playing board.
 */
View* PlayController::createBoard() const {
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = getDisplay()->getWidth();
    rect.h = 400;
    View* board = new BoardView(getModel(), rect);
    board->show();
    return board;
}

/**
 * If it has been 100ms since the last time the player was conveyed, check to see if they're on a conveyor tile.  If so,
 * convey them in the direction of the conveyor tile.
 */
void PlayController::conveyPlayer() const {
    static uint32_t lastConveyance = 0;
    const int ticksBetweenConveyance = 100;
    if (SDL_GetTicks() - lastConveyance >= ticksBetweenConveyance || lastConveyance == 0) {
        getModel()->conveyPlayer();
        lastConveyance = SDL_GetTicks();
    }
}

/**
 * Update the PlayBoardModel.
 * <p>
 * This will convey the player, update the level and check for winning conditions.
 * @param frameDuration
 */
void PlayController::updateModel(long frameDuration) {
    // Move the player along the conveyor belts (if applicable)
    conveyPlayer();

    // Check for level change and winning conditions
    updateLevel(frameDuration);
}

/**
 * Returns true if the model state is PLAY.
 * @return
 */
bool PlayController::checkExitConditions() const {
    return getModel()->getState() == PLAY;
}
