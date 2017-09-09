#include "../controller/PlayController.hpp"
#include "../view/BoardView.hpp"
#include "../uitk/RectangleView.hpp"
#include "../uitk/LabelView.hpp"
#include "../view/TimerView.hpp"
#include "../view/LevelNumberView.hpp"

/**
 * Constructor.
 * <p>
 * Sets the initial value of the time clock. Creates all Views used by the PlayController. Also, sets the starting
 * level.
 * @param model
 * @param display
 * @param startingLevel
 */
PlayController::PlayController(PlayBoardModel* model, Display* display, uint8_t startingLevel) : BoardController(model,
        display) {
    // Initialize the model
    getModel()->setTimeClock(50000);
    getModel()->setLevelNum(startingLevel);

    // Read in the starting level
    getLevelManager()->read();

    // Create each of the views for this controller
    View* board = createBoard();
    getDisplay()->addView("board", board);
    getDisplay()->setFocus("board");

    View* rectangle = createRectangle();
    getDisplay()->addView("rectangle", rectangle);

    View* timer = createTimer();
    getDisplay()->addView("timer", timer);

    View* levelLabel = createLevelLabel();
    getDisplay()->addView("level_number", levelLabel);
}

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
 * Fetch the sub-classed model for this controller.
 * @return the model
 */
PlayBoardModel* PlayController::getModel() const {
    return (PlayBoardModel*) DisplayController::getModel();
}

/**
 * Create a level label for the informational bar.
 */
View* PlayController::createLevelLabel() const {
    SDL_Rect rect = {20, 0, 100, 40};
    rect.y = getDisplay()->getHeight() - rect.h;
    auto levelNumber = new LevelNumberView(getModel(), rect);
    levelNumber->setFontPath(FONT_TELEINDICADORES);
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
    SDL_Rect rect = {0, 0, 100, 40};
    rect.x = getDisplay()->getWidth() - rect.w;
    rect.y = getDisplay()->getHeight() - rect.h;
    auto timer = new TimerView(getModel(), rect);
    timer->setFontPath(FONT_TELEINDICADORES);
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
    SDL_Rect rect = {0, 0, getDisplay()->getWidth(), 40};
    rect.y = getDisplay()->getHeight() - rect.h;
    auto rectangle = new RectangleView(nullptr, rect);
    rectangle->setColor(0);
    rectangle->show();
    return rectangle;
}

/**
 * Create the playing board.
 */
View* PlayController::createBoard() const {
    SDL_Rect rect = {0, 0, getDisplay()->getWidth(), 400};
    auto board = new BoardView(getModel(), rect);

    board->setOnKeyDownCallback([this, board] (SDL_Event event) {
        // User Presses Q
        if (event.key.keysym.sym == SDLK_q) {
            getModel()->setState(QUIT);

        // Move the player down on Down Arrow
        } else if (event.key.keysym.sym == SDLK_DOWN) {
            getModel()->movePlayerInDirection(DIRECTION_DOWN);

        // Move the player up on Up Arrow
        } else if (event.key.keysym.sym == SDLK_UP) {
            getModel()->movePlayerInDirection(DIRECTION_UP);

        // Move the player left on Left Arrow
        } else if (event.key.keysym.sym == SDLK_LEFT) {
            getModel()->movePlayerInDirection(DIRECTION_LEFT);

        // Move the player right on Right Arrow
        } else if (event.key.keysym.sym == SDLK_RIGHT) {
            getModel()->movePlayerInDirection(DIRECTION_RIGHT);
        }
    });

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
bool PlayController::isStillExecuting() const {
    return getModel()->getState() == PLAY;
}
