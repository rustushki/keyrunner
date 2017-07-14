#include "../controller/TitleScreenController.hpp"
#include "../uitk/ImageView.hpp"

/**
 * Constructor.
 * @param model
 * @param display
 */
TitleScreenController::TitleScreenController(BoardModel *model, Display *display) : BaseController(model, display) {
    createKeyImage();
}

/**
 * Destructor.
 */
TitleScreenController::~TitleScreenController() {}

/**
 * Title Screen input.
 */
void TitleScreenController::processInput() {}

/**
 * Update the TitleScreenModel.
 * @param frameDuration
 */
void TitleScreenController::updateModel(long frameDuration) {}

/**
 * Doesn't return false.
 * @return
 */
bool TitleScreenController::checkExitConditions() const {
    return true;
}

/**
 * Create an image view of a large key centered on the title screen.
 */
void TitleScreenController::createKeyImage() {
    SDL_Rect rect;
    rect.x = 30;
    rect.y = 30;
    rect.h = 25;
    rect.w = 25;
    View* keyImage = new ImageView(nullptr, rect, AnimationType::ANIMATION_TYPE_KEY);
    keyImage->show();
    getDisplay()->addView("key", keyImage);
}
