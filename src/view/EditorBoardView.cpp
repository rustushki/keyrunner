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
}

/**
 * Fetch the EditorBoardModel for this EditorBoardView.
 * @return EditorBoardModel*
 */
EditorBoardModel *EditorBoardView::getModel() const {
    return static_cast<EditorBoardModel*>(BoardView::getModel());
}
