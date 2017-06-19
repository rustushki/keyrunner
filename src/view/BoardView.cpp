#include "../view/BoardView.hpp"
#include "../view/AnimationFactory.hpp"

extern AnimationFactory* animationFactory;

/**
 * Constructor.
 * <p>
 * Also creates and houses the key and player animations.
 * @param model
 * @param rect
 */
BoardView::BoardView(PlayModel* model, SDL_Rect rect) : BaseView(model, rect) {
    keyAnimation    = animationFactory->build(ANIMATION_TYPE_KEY);
    playerAnimation = animationFactory->build(ANIMATION_TYPE_PUMPKIN);
}

/**
 * Destructor.
 * <p>
 * Frees the key and player animations.
 */
BoardView::~BoardView() {
    delete keyAnimation;
    delete playerAnimation;
}

/**
 * Redraw the Board based on the tile types in the model.
 * <p>
 * If a tile has the player or the key, draw those too.
 * @param renderer
 */
void BoardView::draw(SDL_Renderer* renderer) {
    const uint16_t tileSize = 25;

    for (uint16_t y = 0; y < PlayModel::GRID_HEIGHT; y++) {
        for (uint16_t x = 0; x < PlayModel::GRID_WIDTH; x++) {
            TileCoord currentTileCoord = TileCoord(x, y);

            TileType tileType = getModel()->getTileType(currentTileCoord);
            AnimationType animationType = tileType.toAnimationType();
            Animation* animation = animationFactory->build(animationType);

            // Determine the coordinate to draw the tile animation
            uint16_t xPosition = x * tileSize;
            uint16_t yPosition = y * tileSize;

            animation->move(xPosition, yPosition);
            animation->blit(renderer);

            // Redraw the Key.
            if (getModel()->tileCoordHasKey(currentTileCoord)) {
                keyAnimation->move(xPosition, yPosition);
                keyAnimation->blit(renderer);
            }

            // Redraw the Player.
            if (getModel()->tileCoordHasPlayer(currentTileCoord)) {
                playerAnimation->move(xPosition, yPosition);
                playerAnimation->blit(renderer);
            }

            delete animation;
        }
    }
}
