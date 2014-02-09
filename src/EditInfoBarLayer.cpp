#include <functional>

#include "EditInfoBarLayer.hpp"
#include "LevelManager.hpp"

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

    // Build the Exit Button.
    exitBl = new ButtonLayer(
          "Exit"
        , 0x333333
        , 0xFF0000
        , blWidth
        , blHeight
    );
    exitBl->setX(r.w - blWidth - margin);
    exitBl->setY(r.h + margin);
    exitBl->setOnClick(std::bind(&EditInfoBarLayer::onExitClick, this));

    // Add the Exit Button to the EditInfoBarLayer.
    addLayer(exitBl);

    // Build the Save Button.
    saveBl = new ButtonLayer(
          "Save"
        , 0x333333
        , 0xFF0000
        , blWidth
        , blHeight
    );
    saveBl->setX(r.w - 2 * blWidth - 2 * margin);
    saveBl->setY(r.h + margin);
    saveBl->setOnClick(std::bind(&EditInfoBarLayer::onSaveClick, this));

    // Add the Save Button to the EditInfoBarLayer.
    addLayer(saveBl);

    // Build the Default Tile Button.
    tileBl = new ButtonLayer(
          "Tile"
        , 0x333333
        , 0xFF0000
        , blWidth
        , blHeight
    );
    tileBl->setX(r.w - 3 * blWidth - 3 * margin);
    tileBl->setY(r.h + margin);
    tileBl->setOnClick(std::bind(&EditInfoBarLayer::onTileClick, this));

    // Add the Save Button to the EditInfoBarLayer.
    addLayer(tileBl);
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
    std::cout << "onTileClick stub called" << std::endl;
}
