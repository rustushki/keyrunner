#include "../view/AnimationFactory.hpp"
#include "../view/GridLayer.hpp"
#include "../view/TileSelectorLayer.hpp"
#include "../model/EditorModel.hpp"
#include "../uitk/ButtonLayer.hpp"
#include "../uitk/ButtonLayerBuilder.hpp"

extern AnimationFactory* animationFactory;

/* ------------------------------------------------------------------------------
 * Constructor.
 *
 * Build a TileSelectorLayer which is a Layer full of image ButtonLayers.  Each
 * ButtonLayer corresponds to a TileType.  The TileTypes are ordered from
 * lowest to highest as they appear in the TileType enum.
 */
TileSelectorLayer::TileSelectorLayer() {
    ButtonLayerBuilder* blb = new ButtonLayerBuilder();

    const uint16_t blWidth  = 40;
    const uint16_t blHeight = 27;
    const uint8_t margin = 4;
    const uint8_t initOffset = 10;

    for (int ttIdx = 0; ttIdx < TileType::length(); ttIdx++) {
        TileType tt = (TileType) ttIdx;
        AnimationType at = tt.toAnimationType();

        // Build the Button for the TileType.
        ButtonLayer* bl = blb
            ->setBgColor(0x333333)
            ->setTextColor(0xFF0000)
            ->setWidth(blWidth)
            ->setHeight(blHeight)
            ->setX((uint16_t) (initOffset + ttIdx * blWidth + ttIdx * margin))
            ->setY((uint16_t) (getRect().y + (getRect().h - blHeight)/2))
            ->setIcon(animationFactory->build(at))
            ->build();

        addLayer(bl);

    }

    Layer::show();

    delete blb;
}

/**
 * React to receiving selection.
 *
 * Upon receiving selection, pass selection to the child which contains the Editor's selected TileType.
 */
void TileSelectorLayer::onSelected() {
    for (uint16_t x = 0; x < getChildCount(); x++) {
        ButtonLayer* buttonLayer = (ButtonLayer*) getChild(x);
        TileType tileType = (TileType) x;
        if (tileType == EditorModel::GetInstance()->getTileType()) {
            buttonLayer->setSelected();
        }
    }
}

/* ------------------------------------------------------------------------------
 * Draws the TileSelectorLayer to the screen.
 *
 * This is mainly a black box, with a gray border.  Inside is a horizontal list
 * of Tiles which may be selected.
 */
void TileSelectorLayer::draw(SDL_Renderer* renderer) {
    const uint8_t  borderWidth = 2;

    SDL_Rect r = getRect();

    // Draw Border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderDrawRect(renderer, &r);

    // Draw the bar at the bottom
    r.x += borderWidth;
    r.y += borderWidth;
    r.w -= 2 * borderWidth;
    r.h -= 2 * borderWidth;
    SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
    SDL_RenderDrawRect(renderer, &r);

    Layer::draw(renderer);

}

/* ------------------------------------------------------------------------------
 * Get the region of the screen upon which this Layer will be drawn.
 *
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

/* ------------------------------------------------------------------------------
 * React to Receiving a Key Down.
 *
 * Escape        - Closes the TileSelectorLayer.
 * Left / Right  - Iterates over the child Layers.
 * Enter         - Sets the EditorModel's TileType based on current child
 *                 ButtonLayer.
 */
void TileSelectorLayer::onKeyDown(SDL_Keycode key) {
    // Escape closes the TileSelectorLayer.
    if (key == SDLK_ESCAPE) {
        hide();

    // Left and Right change the Tile Button selection.
    } else if (key == SDLK_LEFT || key == SDLK_RIGHT) {
        // Figure out which index gets selected next.
        int selChild = getSelectedChildIndex();
        if (selChild >= 0) {
            // Selected Child should go left or right
            if (key == SDLK_LEFT) {
                selChild--;
            } else {
                selChild++;
            }

            // Handle wraparound.
            if (selChild < 0) {
                selChild = getChildCount() - 1;
            } else if (selChild >= getChildCount()) {
                selChild = 0;
            }

            // Change the selected layer.
            getChild(selChild)->setSelected();
        }
    } else if (key == SDLK_RETURN) {
        int selChild = getSelectedChildIndex();
        if (selChild >= 0) {
            EditorModel::GetInstance()->setTileType((TileType) selChild);
            hide();
        }
    }
}
