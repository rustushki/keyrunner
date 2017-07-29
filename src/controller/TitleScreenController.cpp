#include "../controller/TitleScreenController.hpp"
#include "../uitk/ImageView.hpp"
#include "../uitk/LabelView.hpp"
#include "../uitk/MenuView.hpp"

/**
 * Constructor.
 * @param model
 * @param display
 */
TitleScreenController::TitleScreenController(TitleScreenModel* model, Display *display) : BaseController(model,
        display) {
    createKeyImage();
    createKeyRunnerText();
    createPressEnterText();
    createMainMenu();
}

/**
 * Title Screen input.
 * <p>
 * Handles Q key press and the QUIT event.
 */
void TitleScreenController::processInput() {
    SDL_Event event = {};
    while (SDL_PollEvent(&event) == 1 && getModel()->getState() != QUIT) {
        // Key Down Events
        if (event.type == SDL_KEYDOWN) {
            // Q quits the game
            if (event.key.keysym.sym == SDLK_q) {
                getModel()->setState(QUIT);

                // Decrement the Main Menu Cursor on UP
            } else if (event.key.keysym.sym == SDLK_UP) {
                auto mainMenu = dynamic_cast<MenuView *>(getDisplay()->getViewByName("main_menu"));
                mainMenu->decrementCursor();

                // Increment the Main Menu Cursor on DOWN
            } else if (event.key.keysym.sym == SDLK_DOWN) {
                auto mainMenu = dynamic_cast<MenuView *>(getDisplay()->getViewByName("main_menu"));
                mainMenu->incrementCursor();
            }

        // Key Up Events
        } else if (event.type == SDL_KEYUP) {
            // Return
            if (event.key.keysym.sym == SDLK_RETURN) {
                auto mainMenu = dynamic_cast<MenuView *>(getDisplay()->getViewByName("main_menu"));
                auto pressEnterText = getDisplay()->getViewByName("press_enter_text");

                // If the main menu is already visible, pass the key press event to it
                if (mainMenu->isVisible()) {
                    mainMenu->onKeyUp(event);

                // Otherwise, show the main menu
                } else {
                    pressEnterText->hide();
                    mainMenu->show();
                }
            }


        // Quit Events will cause the game to exit
        } else if (event.type == SDL_QUIT) {
            getModel()->setState(QUIT);

        // Delegate Mouse Up, Down, and Hover to the views as they apply
        } else if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN ||
                   event.type == SDL_MOUSEMOTION) {
            auto x = (uint32_t) event.button.x;
            auto y = (uint32_t) event.button.y;
            View *view = getDisplay()->getClickedView(x, y);
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

/**
 * Update the model.
 * @param frameDuration
 */
void TitleScreenController::updateModel(long frameDuration) {
    static_cast<void>(frameDuration);
}

/**
 * Doesn't return false until in the QUIT state.
 * @return
 */
bool TitleScreenController::checkExitConditions() const {
    return getModel()->getState() == TITLE;
}

/**
 * Create an image view of a large key centered on the title screen.
 */
void TitleScreenController::createKeyImage() {
    SDL_Rect rect = {30, 30, 350, 127};
    View* keyImage = new ImageView(nullptr, rect, AnimationType::ANIMATION_TYPE_TITLE_SCREEN_KEY);
    keyImage->show();
    getDisplay()->addView("title_screen_key", keyImage);
}

/**
 * Create the text that says Key Runner.
 */
void TitleScreenController::createKeyRunnerText() {
    SDL_Rect rect = {155, 40, 225, 40};
    auto keyRunner = new LabelView(nullptr, rect);
    keyRunner->setText("Key Runner");
    keyRunner->setFontSize(0);
    keyRunner->setFontPath(FONT_PATH);
    keyRunner->setColor(0x000000);
    keyRunner->setTextColor(0xEEEEEE);
    keyRunner->setMarginVertical(0);
    keyRunner->setMarginHorizontal(0);
    keyRunner->show();
    getDisplay()->addView("key_runner_text", keyRunner);

}

/**
 * Create the text that instructs the user to press enter.
 */
void TitleScreenController::createPressEnterText() {
    SDL_Rect rect = {130, 270, 370, 80};
    auto pressEnterText = new LabelView(nullptr, rect);
    pressEnterText->setText("Press Enter to Continue");
    pressEnterText->setFontSize(0);
    pressEnterText->setFontPath(FONT_PATH);
    pressEnterText->setColor(0x000000);
    pressEnterText->setTextColor(0xEEEEEE);
    pressEnterText->setMarginVertical(0);
    pressEnterText->setMarginHorizontal(0);
    pressEnterText->show();
    getDisplay()->addView("press_enter_text", pressEnterText);

}

/**
 * Create the main menu which is centered and not visible by default.
 */
void TitleScreenController::createMainMenu() {
    SDL_Rect rect = {0, 240, 200, 200};
    rect.x = (getDisplay()->getWidth() - rect.w) / 2;
    auto mainMenu = new MenuView(nullptr, rect);
    mainMenu->setColor(0x000000);
    mainMenu->setOptionBackgroundColor(0x000000);
    mainMenu->setOptionTextColor(0xBBBBBB);
    mainMenu->setOptionCursorTextColor(0xAA3333);
    mainMenu->setVisibleOptionCount(0);
    mainMenu->addOption("Play", [this](SDL_Event event) {
        getModel()->setState(PLAY);
    });
    mainMenu->addOption("Edit", [this](SDL_Event event) {
        getModel()->setState(EDIT);
    });
    mainMenu->addOption("Quit", [this](SDL_Event event) {
        getModel()->setState(QUIT);
    });
    getDisplay()->addView("main_menu", mainMenu);
}
