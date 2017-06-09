#include "Animation.hpp"

/* ------------------------------------------------------------------------------
 * Animation - Build an animation based upon it's type.
 */
Animation::Animation(SpriteSheet* spriteSheet, std::vector<uint16_t> frameList, uint16_t stillsPerSecond) {
    // Build the SpriteSheet.
    this->sheet = spriteSheet;

    // Set the stills per second.
    this->sps = stillsPerSecond;

    // Get List of Frames for this kind of Animation.
    this->frameList = frameList;

    this->currentStill  = 0;
    this->shouldAdvance = false;

    if (this->sps != 0) {
        int fps = 25;
        this->framesPerStill = (uint16_t) (fps / this->sps);
    }

    this->advanceCount = 0;
}

/* ------------------------------------------------------------------------------
 * advance - Increment the advanceCount.  If the advanceCount equals the number
 * of frame required per still, the increment the current frame and blit it in
 * place.
 *
 * Circular wrap the still list.
 *
 * Return true if the still changed due to this advance.
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

uint16_t Animation::getCurrentStill() const {
    return this->currentStill;
}

/* ------------------------------------------------------------------------------
 * blit - Draw the current frame of the animation onto the screen.
 */
void Animation::blit(SDL_Renderer* renderer) {

    // Look into the frame list to determine the logical coordinates of the
    // frame in the sprite sheet.
    uint16_t frameXc = (uint16_t) (this->currentStill * 2 + 0);
    uint16_t frameYc = (uint16_t) (this->currentStill * 2 + 1);
    uint16_t frameX  = this->frameList[frameXc];
    uint16_t frameY  = this->frameList[frameYc];

    SDL_Rect r;
    r.x = this->x;
    r.y = this->y;
    r.w = getWidth();
    r.h = getHeight();

    this->sheet->blitFrame(renderer, frameX, frameY, r);

}

/* ------------------------------------------------------------------------------
 * move - Move the drawing location of this Animation elsewhere.
 */
void Animation::move(uint16_t x, uint16_t y) {
    this->x = x;
    this->y = y;
}

/* ------------------------------------------------------------------------------
 * isAnimating - Return true if the Animation is or can animate.
 */
bool Animation::isAnimating() const {
    return (this->sps != 0 && this->shouldAdvance);
}

/* ------------------------------------------------------------------------------
 * play - Cause an animation to advance stills on advance().
 */
void Animation::play() {
    this->shouldAdvance = true;
}

/* ------------------------------------------------------------------------------
 * stop - Do no advance stills on advance().
 */
void Animation::stop() {
    this->shouldAdvance = false;
}

uint16_t Animation::getWidth() const {
    return sheet->getWidth();
}

uint16_t Animation::getHeight() const {
    return sheet->getHeight();
}
