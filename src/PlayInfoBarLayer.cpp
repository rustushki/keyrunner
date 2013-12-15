#include "PlayInfoBarLayer.hpp"

PlayInfoBarLayer* PlayInfoBarLayer::instance = 0;

PlayInfoBarLayer* PlayInfoBarLayer::GetInstance() {
    if (PlayInfoBarLayer::instance == 0) {
        PlayInfoBarLayer::instance = new PlayInfoBarLayer();
    }

    return PlayInfoBarLayer::instance;
}

PlayInfoBarLayer::PlayInfoBarLayer() {
}

PlayInfoBarLayer::~PlayInfoBarLayer() {
}


