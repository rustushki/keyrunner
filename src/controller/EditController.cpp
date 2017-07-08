#include <sstream>
#include "../controller/EditController.hpp"
#include "../view/EditorBoardView.hpp"
#include "../view/AnimationFactory.hpp"

extern AnimationFactory* animationFactory;

/**
 * Constructor.
 * <p>
 * Creates a new level or loads an existing one depending on the initial state of the model.
 * @param model
 * @param display
 */
EditController::EditController(EditorBoardModel* model, Display* display, Options* options) : BaseController(model, display) {
    // Create New Level for Edit
    if (options->getCreateNewLevel()) {
        getModel()->setLevelNum((uint8_t) (getLevelManager()->getLevelCount() + 1));
        getLevelManager()->create();

        // Load Existing Level for Edit
    } else {
        getModel()->setLevelNum(options->getStartingLevel());
        getLevelManager()->read();
    }

    // Editor Board
    View* board = createBoard();
    getDisplay()->addView("board", board);

    // Black Bar at the Bottom
    View* editInfoBar = createRectangle();
    getDisplay()->addView("edit_info_bar", editInfoBar);

    const uint8_t buttonSpacing = 4;

    // Save Button
    View* saveButton = createSaveButton(board, buttonSpacing);
    getDisplay()->addView("save_button", saveButton);

    // Exit Button
    View* exitButton = createExitButton(board, buttonSpacing);
    getDisplay()->addView("exit_button", exitButton);

    // Tile Selector Buttons
    for (int tileTypeIndex = 0; tileTypeIndex < TileType::length(); tileTypeIndex++) {
        View* tileTypeButton = createTileTypeButton(board, TileType(tileTypeIndex), buttonSpacing);
        std::stringstream viewName;
        viewName << "tile_selector_button_" << tileTypeIndex;
        display->addView(viewName.str(), tileTypeButton);
    }
}

/**
 * Initialize the game elements for edit mode.
 */
void EditController::gameLoop() {
    // Limit to 25 frames per second
    uint32_t fps = 25;
    uint32_t maxDelay = 1000 / fps;

    while (getModel()->getState() != QUIT) {
        // Each iteration represents a frame

        // Begin preparing the frame
        uint32_t workStart = SDL_GetTicks();

        // Advance the animations because we're about to compose the frame
        getDisplay()->advanceAnimations();

        // Build and present the frame
        getDisplay()->draw();

        processInput();

        // Determine how much time we have left after doing work
        uint32_t workEnd = SDL_GetTicks();
        uint32_t workDuration = workEnd - workStart;
        int remainingTime = maxDelay - workDuration;

        // Sleep any remaining time so that we don't hog the CPU
        if (remainingTime > 0) {
            SDL_Delay((uint32_t) remainingTime);
        }
    }
}

/**
 * Handle events for edit mode.
 */
void EditController::processInput() {
    // Wait for an Event
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Key down
        if (event.type == SDL_KEYDOWN) {
            // User Presses Q
            if (event.key.keysym.sym == SDLK_q) {
                getModel()->setState(QUIT);
                break;
            }

        // Handle Mouse Events
        } else if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN ||
                event.type == SDL_MOUSEMOTION) {
            uint32_t x = (uint32_t) event.button.x;
            uint32_t y = (uint32_t) event.button.y;
            const View* view = getDisplay()->getClickedView(x, y);
            if (view != nullptr) {
                if (event.type == SDL_MOUSEBUTTONUP) {
                    view->onMouseUp(event);
                } else if (event.type == SDL_MOUSEBUTTONDOWN){
                    view->onMouseDown(event);
                } else if (event.type == SDL_MOUSEMOTION){
                    view->onMouseHover(event);
                }
            }

        // Handle Quit Event
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
EditorBoardModel *EditController::getModel() const {
    return (EditorBoardModel*) BaseController::getModel();
}

/**
 * Creates a black rectangle.
 * @return View*
 */
View *EditController::createRectangle() const {
    SDL_Rect rect;
    rect.x = 0;
    rect.h = 40;
    rect.y = getDisplay()->getHeight() - rect.h;
    rect.w = getDisplay()->getWidth();
    RectangleView* editInfoBar = new RectangleView(nullptr, rect);
    editInfoBar->setColor(0x0);
    editInfoBar->show();
    return editInfoBar;
}

/**
 * Creates the Editor's BoardView.
 * @return View*
 */
View* EditController::createBoard() const {
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = getDisplay()->getWidth();
    rect.h = 400;
    BoardView* board = new EditorBoardView(getModel(), rect);
    board->setOnMouseHoverCallback([this, board] (SDL_Event event) {
        // Convert the mouse hover coordinates into a tile coordinate
        uint16_t tileX = static_cast<uint16_t>(event.motion.x / board->getTileWidth());
        uint16_t tileY = static_cast<uint16_t>(event.motion.y / board->getTileHeight());
        TileCoord hoverCoordinate(tileX, tileY);

        // Update the model with this new coordinate
        getModel()->setHoverTileCoordinate(hoverCoordinate);
    });
    board->show();
    return board;
}

/**
 * Creates a Save Button.
 * @param board
 * @param buttonSpacing horizontal pixel distance between neighboring buttons
 * @return
 */
View* EditController::createSaveButton(View* board, uint8_t buttonSpacing) const {
    SDL_Rect rect;
    rect.w = 50;
    rect.h = 30;
    rect.x = board->getWidth() - 2 * rect.w - 2 * buttonSpacing;
    rect.y = board->getHeight() + buttonSpacing;
    ButtonView* save = new ButtonView(nullptr, rect);
    save->setText("Save");
    save->setColor(0x333333);
    save->setTextColor(0xFF0000);
    save->setFontPath(FONT_PATH);
    save->setOnMouseUpCallback([this](SDL_Event event) {
        getLevelManager()->write();
    });
    save->show();
    return save;
}

/**
 * Creates the Exit button.
 * @param board
 * @param buttonSpacing horizontal pixel distance between neighboring buttons
 * @return
 */
View *EditController::createExitButton(View *board, int buttonSpacing) const {
    SDL_Rect rect;
    rect.w = 50;
    rect.h = 30;
    rect.x = board->getWidth() - 1 * rect.w - 1 * buttonSpacing;
    rect.y = board->getHeight() + buttonSpacing;
    ButtonView* exit = new ButtonView(nullptr, rect);
    exit->setText("Exit");
    exit->setColor(0x333333);
    exit->setTextColor(0xFF0000);
    exit->setFontPath(FONT_PATH);
    exit->setOnMouseUpCallback([this](SDL_Event event) {
        getModel()->setState(QUIT);
    });
    exit->show();
    return exit;
}

/**
 * Creates a button for the provided tile type and places it horizontally at the bottom of the editor.
 * @param board
 * @param tileType
 * @param buttonSpacing
 * @return
 */
View *EditController::createTileTypeButton(View* board, TileType tileType, uint8_t buttonSpacing) const {
    const uint16_t width  = 40;
    const uint16_t height = 27;
    const uint8_t initialOffset = 10;

    AnimationType animationType = tileType.toAnimationType();
    SDL_Rect rect;
    rect.w = width;
    rect.h = height;
    rect.x = (uint16_t) (initialOffset + ((int) tileType) * width + ((int) tileType) * buttonSpacing);
    rect.y = (uint16_t) ((board->getHeight() + buttonSpacing) + ((40 - buttonSpacing * 2) - height) / 2);
    ButtonView* button = new ButtonView(nullptr, rect);
    button->setColor(0x333333);
    button->setTextColor(0xFF0000);
    button->setIcon(animationFactory->build(animationType));
    button->setOnMouseUpCallback([this, tileType] (SDL_Event event) {
        getModel()->setTileType(tileType);
    });
    button->show();

    return button;
}

