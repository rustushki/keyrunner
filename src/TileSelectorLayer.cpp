#include "ButtonLayer.hpp"
#include "ButtonLayerBuilder.hpp"
#include "TileSelectorLayer.hpp"

TileSelectorLayer::TileSelectorLayer() {
    ButtonLayerBuilder* blb = new ButtonLayerBuilder();

    const uint16_t blWidth  = 50;
    const uint16_t blHeight = 30;
    const uint8_t margin = 4;

    SDL_Rect r = GridLayer::GetInstance()->getRect();

    // Build the Door Button.
    ButtonLayer* bl = blb
        ->setText("button")
        ->setBgColor(0x333333)
        ->setTextColor(0xFF0000)
        ->setWidth(blWidth)
        ->setHeight(blHeight)
        ->setX(r.w - 3 * blWidth - 3 * margin)
        ->setY(r.h + margin)
        ->setIcon(ANIMATION_TYPE_DOOR)
        ->build();

    addLayer(bl);

    delete blb;
}

/* ------------------------------------------------------------------------------
 * draw - Draws the TileSelectorLayer to the screen.  This is mainly a black
 * box, with a gray border.  Inside is a horizontal list of Tiles which may be
 * selected.
 */
void TileSelectorLayer::draw(SDL_Surface* dst) {
    const uint8_t  borderWidth = 2;
    const uint32_t borderColor = SDL_MapRGB(dst->format, 0xAA, 0xAA, 0xAA);
    const uint32_t fillColor   = SDL_MapRGB(dst->format, 0x00, 0x00, 0x00);

    // Build the black bar at the bottom.
    SDL_Rect r = getRect();
    SDL_FillRect(dst, &r, borderColor);

    r.x += borderWidth;
    r.y += borderWidth;
    r.w -= 2 * borderWidth;
    r.h -= 2 * borderWidth;
    SDL_FillRect(dst, &r, fillColor);

    Layer::draw(dst);

}

/* ------------------------------------------------------------------------------
 * getRect - Get the region of the screen upon which this Layer will be drawn.
 * For TileSelectorLayer, it will be at the bottom, taking up some portion of
 * the InfoBarLayer.
 */
SDL_Rect TileSelectorLayer::getRect() const {
    const uint8_t margin = 4;

    SDL_Rect r;
    r.x = margin;
    r.y = GridLayer::GetInstance()->getRect().h + margin;
    r.h = 40 - margin*2;
    r.w = GridLayer::GetInstance()->getRect().w - margin*2;
    return r;
}

