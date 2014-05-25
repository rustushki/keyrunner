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

    exitBl->setOnClick(std::bind(&EditInfoBarLayer::onExitClick, this));

    // Add the Exit Button to the EditInfoBarLayer.
    addLayer(exitBl);

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

    saveBl->setOnClick(std::bind(&EditInfoBarLayer::onSaveClick, this));

    // Add the Save Button to the EditInfoBarLayer.
    addLayer(saveBl);

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

    tileBl->setOnClick(std::bind(&EditInfoBarLayer::onTileClick, this));

    // Add the Save Button to the EditInfoBarLayer.
    addLayer(tileBl);

    // Create the TileSelectorLayer.
    tsl = new TileSelectorLayer();
    tsl->hide();
    addLayer(tsl);

    delete blb;
}

EditInfoBarLayer::~EditInfoBarLayer() {
    delete exitBl;
}

void EditInfoBarLayer::onExitClick() {
    KeyRunner::exitGame();
}

void EditInfoBarLayer::onSaveClick() {
    LevelManager::Write(KeyRunner::getCurrentLevel());
}

void EditInfoBarLayer::onTileClick() {
    tsl->show();
}
