#include "../model/TileCoordinate.hpp"
#include "../view/BoardView.hpp"

/**
 * Constructor.
 * <p>
 * Pre-build animations and begin animating the key, pumpkin and conveyor animations.
 * @param model
 * @param rect
 */
BoardView::BoardView(BoardModel* model, SDL_Rect rect, AnimationFactory* animationFactory) : BaseView(model, rect) {
    setAnimationFactory(animationFactory);

    preBuildAnimations();

    keyAnimation = getAnimationFactory()->build(ANIMATION_TYPE_KEY);
    addAnimation(keyAnimation);

    playerAnimation = getAnimationFactory()->build(ANIMATION_TYPE_PUMPKIN);
    addAnimation(playerAnimation);

    conveyDownAnimation = getAnimationFactory()->build(ANIMATION_TYPE_CONVEY_DOWN);
    addAnimation(conveyDownAnimation);

    conveyUpAnimation = getAnimationFactory()->build(ANIMATION_TYPE_CONVEY_UP);
    addAnimation(conveyUpAnimation);

    conveyLeftAnimation = getAnimationFactory()->build(ANIMATION_TYPE_CONVEY_LEFT);
    addAnimation(conveyLeftAnimation);

    conveyRightAnimation = getAnimationFactory()->build(ANIMATION_TYPE_CONVEY_RIGHT);
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
    for (uint16_t y = 0; y < getModel()->getHeight(); y++) {
        for (uint16_t x = 0; x < getModel()->getWidth(); x++) {
            TileCoordinate currentTileCoord(x, y);

            // Convert the current TileCoordinate's TileType to an AnimationType
            TileType tileType = getModel()->getTileType(currentTileCoord);
            AnimationTypeFactory animationTypeFactory;
            AnimationType animationType = animationTypeFactory.build(tileType);

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
                animation = getPreBuiltAnimations()[animationType];
            }

            // Determine the coordinate to draw the tile animation
            uint16_t xPosition = x * animation->getWidth();
            uint16_t yPosition = y * animation->getHeight();

            animation->move(xPosition, yPosition);
            animation->draw(renderer);
        }
    }

    if (!getModel()->getPlayerHasKey()) {
        Coordinate key = getModel()->getKeyCoord();
        keyAnimation->move(key.getX(), key.getY());
        keyAnimation->draw(renderer);
    }

    Coordinate player = getModel()->getPlayerCoord();
    playerAnimation->move(player.getX(), player.getY());
    playerAnimation->draw(renderer);
}

/**
 * Pre-build the animations so that we don't have to keep building them whenever we need them.
 */
void BoardView::preBuildAnimations() {
    for (int index = 0; index < ANIMATION_TYPE_COUNT; index++) {
        auto animationType = static_cast<AnimationType>(index);
        preBuiltAnimations[animationType] = getAnimationFactory()->build(animationType);
    }
}

/**
 * Free the pre-built animations.
 */
void BoardView::freeAnimations() {
    for (int index = 0; index < ANIMATION_TYPE_COUNT; index++) {
        auto animationType = static_cast<AnimationType>(index);
        Animation* animation = preBuiltAnimations[animationType];
        delete animation;
    }
}

/**
 * Fetch the sub-classed model for this view.
 * @return the model
 */
BoardModel* BoardView::getModel() const {
    return dynamic_cast<BoardModel*>(BaseView::getModel());
}

/**
 * Fetch the map of pre-built animations.
 * <p>
 * This is here so that subclasses can have access to the pre-built animations.
 * @return std::map
 */
std::map<AnimationType, Animation *> BoardView::getPreBuiltAnimations() const {
    return preBuiltAnimations;
}

/**
 * Get the tile width in pixels.
 * @return uint32_t
 */
uint32_t BoardView::getTileWidth() const {
    return getPreBuiltAnimations()[ANIMATION_TYPE_EMPTY]->getWidth();
}

/**
 * Get the tile height in pixels.
 * @return uint32_t
 */
uint32_t BoardView::getTileHeight() const {
    return getPreBuiltAnimations()[ANIMATION_TYPE_EMPTY]->getHeight();
}

void BoardView::setAnimationFactory(AnimationFactory* animationFactory) {
    this->animationFactory = animationFactory;
}

AnimationFactory* BoardView::getAnimationFactory() const {
    return animationFactory;
}
