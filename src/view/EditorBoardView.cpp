#include "../view/EditorBoardView.hpp"

/**
 * Constructor.
 * @param model
 * @param rect
 */
EditorBoardView::EditorBoardView(EditorBoardModel *model, const SDL_Rect &rect) : BoardView(model, rect) {}

/**
 * Draw the board.
 * <p>
 * Also draw the tile position that the mouse is currently hovering over.
 * @param renderer
 */
void EditorBoardView::draw(SDL_Renderer *renderer) {
    // Draw the board first
    BoardView::draw(renderer);

    // Overlay the cursor's tile + a highlight
    drawCursorTile(renderer);
}

/**
 * Draws the currently selected tile type where the mouse is currently hovering plus a color highlight to help
 * distinguish that tile from neighboring tiles of the same type.
 * @param renderer
 */
void EditorBoardView::drawCursorTile(SDL_Renderer* renderer) {
    // Get the pre built animations
    std::map<AnimationType, Animation*> preBuiltAnimations = getPreBuiltAnimations();

    // Convert the Editor's selected tile type into an animation
    TileType selectedTileType = getModel()->getTileType();
    AnimationType animationType = selectedTileType.toAnimationType();
    Animation* animation = preBuiltAnimations[animationType];

    // Convert the Hovered Tile Coordinate into a screen coordinate
    uint16_t xPosition = getModel()->getHoverTileCoordinate().first * animation->getWidth();
    uint16_t yPosition = getModel()->getHoverTileCoordinate().second * animation->getHeight();

    // Draw the animation on top of the hover coordinate
    animation->move(xPosition, yPosition);
    animation->draw(renderer);

    // Draw Highlight
    SDL_Rect highlightRectangle;
    highlightRectangle.x = xPosition;
    highlightRectangle.y = yPosition;
    highlightRectangle.w = animation->getWidth();
    highlightRectangle.h = animation->getHeight();
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawRect(renderer, &highlightRectangle);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0x33);
    SDL_RenderFillRect(renderer, &highlightRectangle);
};

/**
 * Fetch the EditorBoardModel for this EditorBoardView.
 * @return EditorBoardModel*
 */
EditorBoardModel* EditorBoardView::getModel() const {
    return static_cast<EditorBoardModel*>(BoardView::getModel());
}
