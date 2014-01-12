#include <functional>

#include "EditInfoBarLayer.hpp"

EditInfoBarLayer* EditInfoBarLayer::instance = 0;

EditInfoBarLayer* EditInfoBarLayer::GetInstance() {
    if (EditInfoBarLayer::instance == 0) {
        EditInfoBarLayer::instance = new EditInfoBarLayer();
    }

    return EditInfoBarLayer::instance;
}

EditInfoBarLayer::EditInfoBarLayer() {
    const uint16_t exitBlWidth  = 50;
    const uint16_t exitBlHeight = 30;
    const uint8_t margin = 4;

    // Build the Exit Button.
    exitBl = new ButtonLayer(
          "Exit"
        , 0x333333
        , 0xFF0000
        , exitBlWidth
        , exitBlHeight
    );
    exitBl->setX(GridLayer::GetInstance()->getRect().w - exitBlWidth - margin);
    exitBl->setY(GridLayer::GetInstance()->getRect().h + margin);
    exitBl->setOnClick(std::bind(&EditInfoBarLayer::onExitClick, this));

    // Add the Exit Button to the EditInfoBarLayer.
    addLayer(exitBl);
}

EditInfoBarLayer::~EditInfoBarLayer() {
    delete exitBl;
}

void EditInfoBarLayer::onExitClick() {
    KeyRunner::exitGame();
}

