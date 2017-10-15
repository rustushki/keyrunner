#include "../controller/GameOverController.hpp"
#include "../uitk/MenuView.hpp"

/**
 * Constructor.
 * @param model
 * @param display
 */
GameOverController::GameOverController(GameOverModel* model, Display* display) : DisplayController(model, display) {
    auto mainMenu = createMainMenu();
    getDisplay()->addView("main_menu", mainMenu);
    getDisplay()->setFocus("main_menu");

    auto gameOverLabel = createGameOverMenu();
    getDisplay()->addView("game_over_label", gameOverLabel);
}

/**
 * Get the GameOverModel for this controller.
 * @return GameOverModel*
 */
GameOverModel* GameOverController::getModel() const {
    return dynamic_cast<GameOverModel*>(DisplayController::getModel());
}

/**
 * Update the model for this controller.
 * <p>
 * This is currently a no-op.
 * @param frameDuration
 */
void GameOverController::updateModel(long frameDuration) {
    // Do nothing
}

/**
 * The controller will continue to execute so long as the model is in the LOSE state.
 * @return bool
 */
bool GameOverController::isStillExecuting() const {
    return getModel()->getState() == LOSE;
}

/**
 * Create the main menu of the Game Over Screen.
 * @return View*
 */
View* GameOverController::createMainMenu() {
    SDL_Rect rect = {0, 240, 300, 200};
    rect.x = (getDisplay()->getWidth() - rect.w) / 2;
    auto mainMenu = new MenuView(nullptr, rect, getDisplay()->getAnimationFactory());
    mainMenu->setColor(0x000000);
    mainMenu->setOptionBackgroundColor(0x000000);
    mainMenu->setOptionTextColor(0xBBBBBB);
    mainMenu->setOptionCursorTextColor(0xAA3333);
    mainMenu->setVisibleOptionCount(3);
    mainMenu->setOptionFontSize(40);

    // Selecting Try Again will cause the game to restart on the first level
    mainMenu->addOption("Try Again", [this](SDL_Event event) {
        getModel()->setState(PLAY);
    });

    // Selecting Return to Title will return to the title screen
    mainMenu->addOption("Return to Title", [this](SDL_Event event) {
        getModel()->setState(TITLE);
    });

    // Selecting Quit will quit the game
    mainMenu->addOption("Quit", [this](SDL_Event event) {
        getModel()->setState(QUIT);
    });

    // Pressing Q will quit the game
    mainMenu->setOnKeyUpCallback([this](SDL_Event event) {
        if (event.key.keysym.sym == SDLK_q) {
            getModel()->setState(QUIT);
        }
    });

    mainMenu->show();
    return mainMenu;
}

/**
 * Create and return the Game Over text that appears on the the screen.
 * @return View*
 */
View* GameOverController::createGameOverMenu() {
    SDL_Rect rect = {0, 30, 300, 200};
    rect.x = (getDisplay()->getWidth() - rect.w) / 2;
    auto gameOverLabel = new LabelView(nullptr, rect);
    gameOverLabel->setFontSize(0);
    gameOverLabel->setText("Game Over");
    gameOverLabel->setTextColor(0xFF0000);
    gameOverLabel->setFontPath(FONT_CELTIC_HAND);
    gameOverLabel->show();
    return gameOverLabel;
}
