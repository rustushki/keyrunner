#include "../controller/GameWinController.hpp"
#include "../uitk/MenuView.hpp"

/**
 * Constructor.
 * @param model
 * @param display
 */
GameWinController::GameWinController(BaseModel* model, Display* display) : DisplayController(model, display) {
    auto youWinLabel = createYouWinLabel();
    getDisplay()->addView("you_win", youWinLabel);

    auto returnToTitle = createMenu();
    getDisplay()->addView("return_to_title", returnToTitle);
    getDisplay()->setFocus("return_to_title");
}

/**
 * Get the model for this controller.
 * @return GameWinModel*
 */
GameWinModel* GameWinController::getModel() const {
    return dynamic_cast<GameWinModel*>(DisplayController::getModel());
}

/**
 * Update the model for this controller.
 * @param frameDuration
 */
void GameWinController::updateModel(long frameDuration) {
    // Do nothing
}

/**
 * Returns true if the model state is still WIN.
 * @return bool
 */
bool GameWinController::isStillExecuting() const {
    return getModel()->getState() == WIN;
}

/**
 * Creates a menu which allows the user to return to title or quit the game.
 * @return View*
 */
View* GameWinController::createMenu() const {
    SDL_Rect rect = {0, 240, 200, 200};
    rect.x = (getDisplay()->getWidth() - rect.w) / 2;
    auto returnToTitle = new MenuView(nullptr, rect, getDisplay()->getAnimationFactory());
    returnToTitle->setColor(0x000000);
    returnToTitle->setOptionBackgroundColor(0x000000);
    returnToTitle->setOptionTextColor(0xBBBBBB);
    returnToTitle->setOptionCursorTextColor(0xAA3333);
    returnToTitle->setVisibleOptionCount(0);
    returnToTitle->setOptionFontSize(30);
    returnToTitle->show();

    returnToTitle->addOption("Return to Title", [this](SDL_Event event) {
        getModel()->setState(TITLE);
    });

    returnToTitle->addOption("Quit", [this](SDL_Event event) {
        getModel()->setState(QUIT);
    });

    returnToTitle->setOnKeyUpCallback([this](SDL_Event event) {
        // Q quits the game
        if (event.key.keysym.sym == SDLK_q) {
            getModel()->setState(QUIT);
        }
    });

    return returnToTitle;
}

/**
 * Creates a label that tells the player they won.
 * @return View*
 */
View* GameWinController::createYouWinLabel() const {
    SDL_Rect rect = {0, 30, 300, 200};
    rect.x = (getDisplay()->getWidth() - rect.w) / 2;
    auto youWinLabel = new LabelView(nullptr, rect);
    youWinLabel->setFontSize(0);
    youWinLabel->setText("You Win!");
    youWinLabel->setTextColor(0xFFFFFF);
    youWinLabel->setFontPath(FONT_CELTIC_HAND);
    youWinLabel->show();
    return youWinLabel;
}
