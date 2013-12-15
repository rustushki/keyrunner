#include "PlayInfoBarLayer.hpp"
#include "PlayRootLayer.hpp"

PlayRootLayer* PlayRootLayer::instance = 0;

PlayRootLayer::PlayRootLayer() : RootLayer(PlayInfoBarLayer::GetInstance()) {
}

PlayRootLayer* PlayRootLayer::GetInstance() {
    if (instance == 0) {
        instance = new PlayRootLayer();
    }

    return instance;
}
