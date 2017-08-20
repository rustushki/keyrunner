#include "../controller/GameOverController.hpp"
#include "../uitk/MenuView.hpp"

/**
 * Constructor.
 * @param model
 * @param display
 */
GameOverController::GameOverController(GameOverModel *model, Display *display) : DisplayController(model, display) {
    auto mainMenu = createMainMenu();
    getDisplay()->addView("main_menu", mainMenu);
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
View *GameOverController::createMainMenu() {
    SDL_Rect rect = {0, 240, 200, 200};
    rect.x = (getDisplay()->getWidth() - rect.w) / 2;
    auto mainMenu = new MenuView(nullptr, rect);
    mainMenu->setColor(0x000000);
    mainMenu->setOptionBackgroundColor(0x000000);
    mainMenu->setOptionTextColor(0xBBBBBB);
    mainMenu->setOptionCursorTextColor(0xAA3333);
    mainMenu->setVisibleOptionCount(3);
    mainMenu->addOption("Try Again", [](SDL_Event event) {});
    mainMenu->addOption("Return to Title", [this](SDL_Event event) {
        getModel()->setState(TITLE);
    });

    mainMenu->addOption("Quit", [this](SDL_Event event) {
        getModel()->setState(QUIT);
    });

    mainMenu->show();
    return mainMenu;
}
