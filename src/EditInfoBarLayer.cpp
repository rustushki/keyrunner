#include "EditInfoBarLayer.hpp"

EditInfoBarLayer* EditInfoBarLayer::instance = 0;

EditInfoBarLayer* EditInfoBarLayer::GetInstance() {
    if (EditInfoBarLayer::instance == 0) {
        EditInfoBarLayer::instance = new EditInfoBarLayer();
    }

    return EditInfoBarLayer::instance;
}

EditInfoBarLayer::EditInfoBarLayer() {
    bl = new ButtonLayer("Tile", 0x333333, 0xFF0000, 50, 50);
    addLayer(bl);
}

EditInfoBarLayer::~EditInfoBarLayer() {
    delete bl;
}
