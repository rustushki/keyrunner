#include "../uitk/Animation.hpp"

/**
 * Constructor.
 * <p>
 * An animation is composed of a SpriteSheet, list of frames within that sheet, and a frame rate.
 * @param spriteSheet
 * @param frameList
 * @param stillsPerSecond
 */
Animation::Animation(SpriteSheet* spriteSheet, std::vector<uint16_t> frameList, uint16_t stillsPerSecond) {
    // Build the SpriteSheet.
    this->sheet = spriteSheet;

    // Set the stills per second.
    this->sps = stillsPerSecond;

    // Get List of Frames for this kind of Animation.
    this->frameList = std::move(frameList);

    this->currentStill  = 0;
    this->shouldAdvance = false;

    if (this->sps != 0) {
        int fps = 25;
        this->framesPerStill = (uint16_t) (fps / this->sps);
    }

    this->advanceCount = 0;
}

/**
 * Increment the advanceCount.
 * <p>
 * If the advanceCount equals the number of frame required per still, the increment the current frame and draw it in
 * place. Circular wrap the still list.
 *
 * @return boolean true if the still changed due to this advance.
 */
bool Animation::advance() {

    bool advanced = false;

    if (this->shouldAdvance) {

        this->advanceCount++;

        if (this->advanceCount == framesPerStill) {

            this->advanceCount = 0;
            advanced = true;
            this->currentStill++;


            if (this->currentStill >= this->frameList.size() / 2) {
                this->currentStill = 0;
            }
        }
    }

    return advanced;
}

/**
 * Draw the current frame of the animation onto the screen.
 * @param SDL_Renderer*
 */
void Animation::draw(SDL_Renderer* renderer) {
    // Look into the frame list to determine the logical coordinates of the
    // frame in the sprite sheet.
    auto frameXc = static_cast<uint16_t>(this->currentStill * 2 + 0);
    auto frameYc = static_cast<uint16_t>(this->currentStill * 2 + 1);
    auto frameX  = this->frameList[frameXc];
    auto frameY  = this->frameList[frameYc];

    SDL_Rect destination = {static_cast<uint16_t>(x), static_cast<uint16_t>(y), getWidth(), getHeight()};
    this->sheet->drawStill(renderer, frameX, frameY, destination);
}

/**
 * Move the drawing location of this Animation elsewhere.
 */
void Animation::move(long x, long y) {
    this->x = x;
    this->y = y;
}

/**
 * Is the animation animating?
 * <p>
 * Returns true if the SPS is greater than 0 or if the animation is set to advance (i.e. play() was called).
 * @return boolean
 */
bool Animation::isAnimating() const {
    return (this->sps != 0 && this->shouldAdvance);
}

/**
 * Cause an animation to advance stills on advance().
 */
void Animation::play() {
    this->shouldAdvance = true;
}

/**
 * Do not advance stills on advance().
 */
void Animation::stop() {
    this->shouldAdvance = false;
}

/**
 * Get the frame width of the animation.
 * @return
 */
uint16_t Animation::getWidth() const {
    return sheet->getWidth();
}

/**
 * Get the frame height of the animation.
 * @return
 */
uint16_t Animation::getHeight() const {
    return sheet->getHeight();
}
