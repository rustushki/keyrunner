#include "../controller/TitleScreenController.hpp"
#include "../uitk/ImageView.hpp"
#include "../uitk/LabelView.hpp"

/**
 * Constructor.
 * @param model
 * @param display
 */
TitleScreenController::TitleScreenController(BoardModel *model, Display *display) : BaseController(model, display) {
    createKeyImage();
    createKeyRunnerText();
    createPressEnterText();
}

/**
 * Destructor.
 */
TitleScreenController::~TitleScreenController() {}

/**
 * Title Screen input.
 * <p>
 * Handles Q key press and the QUIT event.
 */
void TitleScreenController::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            // User Presses Q
            if (event.key.keysym.sym == SDLK_q) {
                getModel()->setState(QUIT);
                break;
            }
        // Handle Quit Event.
        } else if (event.type == SDL_QUIT) {
            getModel()->setState(QUIT);
            break;
        }
    }
}

/**
 * Update the TitleScreenModel.
 * @param frameDuration
 */
void TitleScreenController::updateModel(long frameDuration) {}

/**
 * Doesn't return false until in the QUIT state.
 * @return
 */
bool TitleScreenController::checkExitConditions() const {
    return getModel()->getState() != QUIT;
}

/**
 * Create an image view of a large key centered on the title screen.
 */
void TitleScreenController::createKeyImage() {
    SDL_Rect rect;
    rect.x = 30;
    rect.y = 30;
    rect.w = 350;
    rect.h = 127;
    View* keyImage = new ImageView(nullptr, rect, AnimationType::ANIMATION_TYPE_TITLE_SCREEN_KEY);
    keyImage->show();
    getDisplay()->addView("title_screen_key", keyImage);
}

/**
 * Create the text that says Key Runner.
 */
void TitleScreenController::createKeyRunnerText() {
    SDL_Rect rect;
    rect.x = 155;
    rect.y = 40;
    rect.w = 225;
    rect.h = 40;
    LabelView* keyRunner = new LabelView(nullptr, rect);
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
    SDL_Rect rect;
    rect.x = 130;
    rect.y = 270;
    rect.w = 370;
    rect.h = 80;
    LabelView* pressEnterText = new LabelView(nullptr, rect);
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
