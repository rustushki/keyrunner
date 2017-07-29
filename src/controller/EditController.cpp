#include <sstream>
#include "../controller/EditController.hpp"
#include "../view/EditorBoardView.hpp"
#include "../view/AnimationFactory.hpp"
#include "../model/AnimationTypeFactory.hpp"

extern AnimationFactory* animationFactory;

/**
 * Constructor.
 * <p>
 * Creates a new level or loads an existing one depending on the initial state of the model.
 * @param model
 * @param display
 */
EditController::EditController(EditorBoardModel* model, Display* display, Options* options) : BoardController(model,
        display) {

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
    for (int tileTypeIndex = 0; tileTypeIndex < static_cast<int>(TileType::Count); tileTypeIndex++) {
        View* tileTypeButton = createTileTypeButton(board, TileType(tileTypeIndex), buttonSpacing);
        std::stringstream viewName;
        viewName << "tile_selector_button_" << tileTypeIndex;
        display->addView(viewName.str(), tileTypeButton);
    }
}

/**
 * Handle events for edit mode.
 */
void EditController::processInput() {
    // Wait for an Event
    SDL_Event event = {};
    while (SDL_PollEvent(&event) == 1) {
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
            auto x = static_cast<uint32_t>(event.button.x);
            auto y = static_cast<uint32_t>(event.button.y);
            View* view = getDisplay()->getViewUnderCoordinate(x, y);
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
View* EditController::createRectangle() const {
    SDL_Rect rect = {};
    rect.x = 0;
    rect.h = 40;
    rect.y = getDisplay()->getHeight() - rect.h;
    rect.w = getDisplay()->getWidth();
    auto editInfoBar = new RectangleView(nullptr, rect);
    editInfoBar->setColor(0x0);
    editInfoBar->show();
    return editInfoBar;
}

/**
 * Creates the Editor's BoardView.
 * @return View*
 */
View* EditController::createBoard() const {
    SDL_Rect rect = {0, 0, getDisplay()->getWidth(), 400};
    BoardView* board = new EditorBoardView(getModel(), rect);

    board->setOnMouseHoverCallback([this, board] (SDL_Event event) {
        // Convert the mouse hover coordinates into a tile coordinate
        auto tileX = static_cast<uint16_t>(event.motion.x / board->getTileWidth());
        auto tileY = static_cast<uint16_t>(event.motion.y / board->getTileHeight());
        TileCoord hoverCoordinate(tileX, tileY);

        // If replacing, the selected tile type will overwrite the hovered over tile
        HoverBehavior hoverBehavior = getModel()->getHoverBehavior();
        if (hoverBehavior == HoverBehavior::REPLACE) {
            getModel()->changeTileType(hoverCoordinate, getModel()->getTileType());
            getModel()->setHoverTileCoordinate(TileCoord(getModel()->getWidth(), getModel()->getHeight()));

        // If pointing, the selected tile type will hover with the mouse
        } else if (hoverBehavior == HoverBehavior::POINT) {
            getModel()->setHoverTileCoordinate(hoverCoordinate);

        // If moving key, move the key to the current hover coordinate
        } else if (hoverBehavior == HoverBehavior::MOVE_KEY) {
            getModel()->setKeyCoord(hoverCoordinate);

        // If moving player, move the key to the current hover coordinate
        } else if (hoverBehavior == HoverBehavior::MOVE_PLAYER) {
            getModel()->setPlayerCoord(hoverCoordinate);
        }
    });

    board->setOnMouseDownCallback([this, board] (SDL_Event event) {
        auto tileX = static_cast<uint16_t>(event.button.x / board->getTileWidth());
        auto tileY = static_cast<uint16_t>(event.button.y / board->getTileHeight());
        TileCoord mouseCoordinate(tileX, tileY);

        // If the key is at the mouse coordinate, hover will now move the key
        if (getModel()->getKeyCoord() == mouseCoordinate) {
            getModel()->setHoverBehavior(HoverBehavior::MOVE_KEY);
            getModel()->setHoverTileCoordinate(TileCoord(getModel()->getWidth(), getModel()->getHeight()));

        // If the key is at the mouse coordinate, hover will now move the key
        } else if (getModel()->getPlayerCoord() == mouseCoordinate) {
            getModel()->setHoverBehavior(HoverBehavior::MOVE_PLAYER);
            getModel()->setHoverTileCoordinate(TileCoord(getModel()->getWidth(), getModel()->getHeight()));

        // Otherwise, hover will now replace tiles, and go ahead and replace the current tile so that a mouse move is
        // not required to affect change
        } else {
            getModel()->setHoverBehavior(HoverBehavior::REPLACE);
            getModel()->changeTileType(mouseCoordinate, getModel()->getTileType());
        }
    });

    board->setOnMouseUpCallback([this, board] (SDL_Event event) {
        // Hover will go back to only showing the selected tile type
        getModel()->setHoverBehavior(HoverBehavior::POINT);
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
    SDL_Rect rect = {0, 0, 50, 30};
    rect.x = board->getWidth() - 2 * rect.w - 2 * buttonSpacing;
    rect.y = board->getHeight() + buttonSpacing;
    auto save = new ButtonView(nullptr, rect);
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
    SDL_Rect rect = {0, 0, 50, 30};
    rect.x = board->getWidth() - 1 * rect.w - 1 * buttonSpacing;
    rect.y = board->getHeight() + buttonSpacing;
    auto exit = new ButtonView(nullptr, rect);
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

    AnimationTypeFactory animationTypeFactory;
    AnimationType animationType = animationTypeFactory.build(tileType);
    SDL_Rect rect = {0, 0, width, height};
    rect.x = (uint16_t) (initialOffset + ((int) tileType) * width + ((int) tileType) * buttonSpacing);
    rect.y = (uint16_t) ((board->getHeight() + buttonSpacing) + ((40 - buttonSpacing * 2) - height) / 2);
    auto button = new ButtonView(nullptr, rect);
    button->setColor(0x333333);
    button->setTextColor(0xFF0000);
    button->setIcon(animationFactory->build(animationType));
    button->setOnMouseUpCallback([this, tileType] (SDL_Event event) {
        getModel()->setTileType(tileType);
    });
    button->show();

    return button;
}

/**
 * Update the EditorBoardModel.
 * @param frameDuration
 */
void EditController::updateModel(long frameDuration) {}

/**
 * Returns true if the state is QUIT.
 * @return
 */
bool EditController::checkExitConditions() const {
    return getModel()->getState() != QUIT;
}
