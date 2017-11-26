#include "../view/EditorBoardView.hpp"

/**
 * Constructor.
 * @param model
 * @param rect
 */
EditorBoardView::EditorBoardView(EditorBoardModel* model, const SDL_Rect &rect, AnimationFactory* animationFactory) :
        BoardView(model, rect, animationFactory) {}

/**
 * Draw the board.
 * <p>
 * Also draw the tile position that the mouse is currently hovering over.
 * @param renderer
 */
void EditorBoardView::draw(SDL_Renderer* renderer) {
    // Draw the board first
    BoardView::draw(renderer);

    // Determine if we should draw just the highlight, or if we should include the tile type, too
    TileCoordinate hoverTileCoord = getModel()->getHoverTileCoordinate();
    bool justHighlight = false;
    // TODO:
    //justHighlight |= (getModel()->getKeyCoord() == hoverTileCoord);
    //justHighlight |= (getModel()->getPlayerCoord() == hoverTileCoord);

    // Draw the highlight and maybe the cursor tile
    drawCursorTile(renderer, justHighlight);
}

/**
 * Draws the currently selected tile type where the mouse is currently hovering plus a color highlight to help
 * distinguish that tile from neighboring tiles of the same type.
 * @param renderer
 */
void EditorBoardView::drawCursorTile(SDL_Renderer* renderer, bool justHighlight) {
    // Get the pre built animations
    std::map<AnimationType, Animation*> preBuiltAnimations = getPreBuiltAnimations();

    // Convert the Editor's selected tile type into an animation
    TileType selectedTileType = getModel()->getTileType();
    AnimationTypeFactory animationTypeFactory;
    AnimationType animationType = animationTypeFactory.build(selectedTileType);
    Animation* animation = preBuiltAnimations[animationType];

    // Convert the Hovered Tile Coordinate into a screen coordinate
    uint16_t xPosition = static_cast<uint16_t>(getModel()->getHoverTileCoordinate().getX() * animation->getWidth());
    uint16_t yPosition = static_cast<uint16_t>(getModel()->getHoverTileCoordinate().getY() * animation->getHeight());

    // Draw the animation on top of the hover coordinate, unless we're just going to draw the highlight
    if (!justHighlight) {
        animation->move(xPosition, yPosition);
        animation->draw(renderer);
    }

    // Draw Highlight
    SDL_Rect where = {xPosition, yPosition, animation->getWidth(), animation->getHeight()};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawRect(renderer, &where);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0x33);
    SDL_RenderFillRect(renderer, &where);
};

/**
 * Fetch the EditorBoardModel for this EditorBoardView.
 * @return EditorBoardModel*
 */
EditorBoardModel* EditorBoardView::getModel() const {
    return dynamic_cast<EditorBoardModel*>(BoardView::getModel());
}
