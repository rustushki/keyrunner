#include "../view/BoardView.hpp"
#include "../view/AnimationFactory.hpp"

extern AnimationFactory* animationFactory;

/**
 * Constructor.
 * <p>
 * Pre-build animations and begin animate the key and pumpkin.
 * @param model
 * @param rect
 */
BoardView::BoardView(PlayModel* model, SDL_Rect rect) : BaseView(model, rect) {
    preBuildAnimations();

    keyAnimation = animationFactory->build(ANIMATION_TYPE_KEY);
    addAnimation(keyAnimation);

    playerAnimation = animationFactory->build(ANIMATION_TYPE_PUMPKIN);
    addAnimation(playerAnimation);
}

/**
 * Destructor.
 */
BoardView::~BoardView() {
    freeAnimations();
}

/**
 * Redraw the Board based on the tile types in the model.
 * <p>
 * If a tile has the player or the key, draw those too.
 * @param renderer
 */
void BoardView::draw(SDL_Renderer* renderer) {
    const uint16_t tileSize = 25;

    for (uint16_t y = 0; y < getModel()->getGridHeight(); y++) {
        for (uint16_t x = 0; x < getModel()->getGridWidth(); x++) {
            TileCoord currentTileCoord = TileCoord(x, y);

            TileType tileType = getModel()->getTileType(currentTileCoord);
            AnimationType animationType = tileType.toAnimationType();
            Animation* animation = preBuiltAnimations[animationType];

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
        }
    }
}

/**
 * Pre-build the animations so that we don't have to keep building them whenever we need them.
 */
void BoardView::preBuildAnimations() {
    for (int index = 0; index < ANIMATION_TYPE_COUNT; index++) {
        AnimationType animationType = (AnimationType) index;
        preBuiltAnimations[animationType] = animationFactory->build(animationType);
    }
}

/**
 * Free the pre-built animations.
 */
void BoardView::freeAnimations() {
    for (int index = 0; index < ANIMATION_TYPE_COUNT; index++) {
        AnimationType animationType = (AnimationType) index;
        Animation* animation = preBuiltAnimations[animationType];
        delete animation;
    }
}
