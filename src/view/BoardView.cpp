#include "../view/BoardView.hpp"
#include "../view/AnimationFactory.hpp"

extern AnimationFactory* animationFactory;

/**
 * Constructor.
 * <p>
 * Pre-build animations and begin animating the key, pumpkin and conveyor animations.
 * @param model
 * @param rect
 */
BoardView::BoardView(BoardModel* model, SDL_Rect rect) : BaseView(model, rect) {
    preBuildAnimations();

    keyAnimation = animationFactory->build(ANIMATION_TYPE_KEY);
    addAnimation(keyAnimation);

    playerAnimation = animationFactory->build(ANIMATION_TYPE_PUMPKIN);
    addAnimation(playerAnimation);

    conveyDownAnimation = animationFactory->build(ANIMATION_TYPE_CONVEY_DOWN);
    addAnimation(conveyDownAnimation);

    conveyUpAnimation = animationFactory->build(ANIMATION_TYPE_CONVEY_UP);
    addAnimation(conveyUpAnimation);

    conveyLeftAnimation = animationFactory->build(ANIMATION_TYPE_CONVEY_LEFT);
    addAnimation(conveyLeftAnimation);

    conveyRightAnimation = animationFactory->build(ANIMATION_TYPE_CONVEY_RIGHT);
    addAnimation(conveyRightAnimation);
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

    for (uint16_t y = 0; y < getModel()->getHeight(); y++) {
        for (uint16_t x = 0; x < getModel()->getWidth(); x++) {
            TileCoord currentTileCoord = TileCoord(x, y);

            // Convert the current TileCoord's TileType to an AnimationType
            TileType tileType = getModel()->getTileType(currentTileCoord);
            AnimationType animationType = tileType.toAnimationType();

            // Get the pre-built animation associated with AnimationType
            Animation* animation;

            // If the AnimationType is that of a conveyor, use the corresponding conveyor Animation (which is currently
            // animating).  Otherwise, use a pre-built animation.
            if (animationType == ANIMATION_TYPE_CONVEY_DOWN) {
                animation = conveyDownAnimation;
            } else if (animationType == ANIMATION_TYPE_CONVEY_UP) {
                animation = conveyUpAnimation;
            } else if (animationType == ANIMATION_TYPE_CONVEY_LEFT) {
                animation = conveyLeftAnimation;
            } else if (animationType == ANIMATION_TYPE_CONVEY_RIGHT) {
                animation = conveyRightAnimation;
            } else {
                animation = preBuiltAnimations[animationType];
            }

            // Determine the coordinate to draw the tile animation
            uint16_t xPosition = x * tileSize;
            uint16_t yPosition = y * tileSize;

            animation->move(xPosition, yPosition);
            animation->draw(renderer);

            // Redraw the Key.
            if (getModel()->tileCoordHasKey(currentTileCoord)) {
                keyAnimation->move(xPosition, yPosition);
                keyAnimation->draw(renderer);
            }

            // Redraw the Player.
            if (getModel()->tileCoordHasPlayer(currentTileCoord)) {
                playerAnimation->move(xPosition, yPosition);
                playerAnimation->draw(renderer);
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

/**
 * Fetch the sub-classed model for this view.
 * @return the model
 */
BoardModel* BoardView::getModel() const {
    return (BoardModel*) BaseView::getModel();
}
