#include <sstream>
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
    createEditorLevelSelectorMenu();
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

    pressEnterText->setOnKeyUpCallback([this, pressEnterText](SDL_Event event) {
        if (event.key.keysym.sym == SDLK_RETURN) {
            auto mainMenu = dynamic_cast<MenuView *>(getDisplay()->getViewByName("main_menu"));
            pressEnterText->hide();
            mainMenu->show();
            getDisplay()->setFocus("main_menu");
        }
    });

    getDisplay()->addView("press_enter_text", pressEnterText);
    getDisplay()->setFocus("press_enter_text");
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
        auto levelEditorMenuName = "level_selector_menu";
        getDisplay()->setFocus(levelEditorMenuName);
        getDisplay()->getViewByName(levelEditorMenuName)->show();
        getDisplay()->getViewByName("main_menu")->show();
    });

    mainMenu->addOption("Quit", [this](SDL_Event event) {
        getModel()->setState(QUIT);
    });

    mainMenu->setOnKeyUpCallback([this, mainMenu](SDL_Event event) {
        // Q quits the game
        if (event.key.keysym.sym == SDLK_q) {
            getModel()->setState(QUIT);

        // Decrement the Main Menu Cursor on UP
        } else if (event.key.keysym.sym == SDLK_UP) {
            mainMenu->decrementCursor();

        // Increment the Main Menu Cursor on DOWN
        } else if (event.key.keysym.sym == SDLK_DOWN) {
            mainMenu->incrementCursor();
        }
    });

    getDisplay()->addView("main_menu", mainMenu);
}

/**
 * Create the Level Selector Menu.
 * <p>
 * This menu lets you start creating a new level OR allows you to edit an existing level. Alternately, you may select
 * the Back option to go back to the Main Menu.
 */
void TitleScreenController::createEditorLevelSelectorMenu() {
    SDL_Rect rect = {0, 240, 200, 200};
    rect.x = (getDisplay()->getWidth() - rect.w) / 2;
    auto levelEditorMenu = new MenuView(nullptr, rect);
    levelEditorMenu->setColor(0x000000);
    levelEditorMenu->setOptionBackgroundColor(0x000000);
    levelEditorMenu->setOptionTextColor(0xBBBBBB);
    levelEditorMenu->setOptionCursorTextColor(0xAA3333);
    levelEditorMenu->setVisibleOptionCount(5);

    levelEditorMenu->addOption("New", [this](SDL_Event event) {});
    levelEditorMenu->addOption("Back", [this](SDL_Event event) {});
    for (int levelNumber = 1; levelNumber <= getModel()->getMaxLevel(); levelNumber++) {
        std::stringstream levelName;
        levelName << "Level" << " " << levelNumber;
        levelEditorMenu->addOption(levelName.str(), [this](SDL_Event event) {});
    }

    levelEditorMenu->setOnKeyUpCallback([this, levelEditorMenu](SDL_Event event) {
        // Q quits the game
        if (event.key.keysym.sym == SDLK_q) {
            getModel()->setState(QUIT);

            // Decrement the Main Menu Cursor on UP
        } else if (event.key.keysym.sym == SDLK_UP) {
            levelEditorMenu->decrementCursor();

            // Increment the Main Menu Cursor on DOWN
        } else if (event.key.keysym.sym == SDLK_DOWN) {
            levelEditorMenu->incrementCursor();
        }
    });

    getDisplay()->addView("level_selector_menu", levelEditorMenu);
}

/**
 * Fetch the sub-classed model for this controller.
 * @return the model
 */
TitleScreenModel *TitleScreenController::getModel() const {
    return dynamic_cast<TitleScreenModel*>(BaseController::getModel());
}
