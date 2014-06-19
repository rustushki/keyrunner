#include <functional>

#include "ButtonLayerBuilder.hpp"
#include "EditInfoBarLayer.hpp"
#include "LevelManager.hpp"
#include "TileSelectorLayer.hpp"

EditInfoBarLayer* EditInfoBarLayer::instance = 0;

EditInfoBarLayer* EditInfoBarLayer::GetInstance() {
    if (EditInfoBarLayer::instance == 0) {
        EditInfoBarLayer::instance = new EditInfoBarLayer();
    }

    return EditInfoBarLayer::instance;
}

EditInfoBarLayer::EditInfoBarLayer() {
    const uint16_t blWidth  = 50;
    const uint16_t blHeight = 30;
    const uint8_t margin = 4;
    SDL_Rect r = GridLayer::GetInstance()->getRect();

    ButtonLayerBuilder* blb = new ButtonLayerBuilder();

    // Build the Tile Button.
    tileBl = blb
        ->setText("Tile")
        ->setBgColor(0x333333)
        ->setTextColor(0xFF0000)
        ->setWidth(blWidth)
        ->setHeight(blHeight)
        ->setX(r.w - 3 * blWidth - 3 * margin)
        ->setY(r.h + margin)
        ->build();

    tileBl->setOnEnter(std::bind(&EditInfoBarLayer::onTileEnter, this));

    // Add the Save Button to the EditInfoBarLayer.
    addLayer(tileBl);

    // Build the Save Button.
    saveBl = blb
        ->setText("Save")
        ->setBgColor(0x333333)
        ->setTextColor(0xFF0000)
        ->setWidth(blWidth)
        ->setHeight(blHeight)
        ->setX(r.w - 2 * blWidth - 2 * margin)
        ->setY(r.h + margin)
        ->build();

    saveBl->setOnEnter(std::bind(&EditInfoBarLayer::onSaveEnter, this));

    // Add the Save Button to the EditInfoBarLayer.
    addLayer(saveBl);

    // Build the Exit Button.
    exitBl = blb
        ->setText("Exit")
        ->setBgColor(0x333333)
        ->setTextColor(0xFF0000)
        ->setWidth(blWidth)
        ->setHeight(blHeight)
        ->setX(r.w - blWidth - margin)
        ->setY(r.h + margin)
        ->build();

    exitBl->setOnEnter(std::bind(&EditInfoBarLayer::onExitEnter, this));

    // Add the Exit Button to the EditInfoBarLayer.
    addLayer(exitBl);

    // Create the TileSelectorLayer.
    tsl = new TileSelectorLayer();
    tsl->hide();
    addLayer(tsl);

    delete blb;
}

EditInfoBarLayer::~EditInfoBarLayer() {
    delete exitBl;
}

void EditInfoBarLayer::onExitEnter() {
    KeyRunner::exitGame();
}

void EditInfoBarLayer::onSaveEnter() {
    LevelManager::Write(KeyRunner::getCurrentLevel());
}

void EditInfoBarLayer::onTileEnter() {
    tsl->show();
}

/* ------------------------------------------------------------------------------
 * onKeyDown - ESC should close the tile selector layer.
 */
void EditInfoBarLayer::onKeyDown(SDLKey key) {
    // Escape closes the TileSelectorLayer.
    if (key == SDLK_ESCAPE) {
        hide();

    // Left and Right change the Tile Button selection.
    } else if (key == SDLK_LEFT || key == SDLK_RIGHT) {
        // Figure out which index gets selected next.
        int selChild = getSelectedChildIndex();
        int origChild = selChild;
        if (selChild >= 0) {
            do {
                // Selected Child should go left or right
                if (key == SDLK_LEFT) {
                    selChild--;
                } else if (key == SDLK_RIGHT) {
                    selChild++;
                }

                // Handle wraparound.
                if (selChild < 0) {
                    selChild = getChildCount() - 1;
                } else if (selChild >= getChildCount()) {
                    selChild = 0;
                }

                // But only to a visible to the next visible child.
            } while (!getChild(selChild)->isVisible() && origChild != selChild);

            // Change the selected layer.
            getChild(selChild)->setSelected();
        }
    } else if (key == SDLK_RETURN) {
        getChild(getSelectedChildIndex())->onEnter();
    }
}
