#include "EditInfoBarLayer.hpp"
#include "EditRootLayer.hpp"

EditRootLayer* EditRootLayer::instance = 0;

EditRootLayer::EditRootLayer() : RootLayer(EditInfoBarLayer::GetInstance()) {
}

EditRootLayer* EditRootLayer::GetInstance() {
    if (instance == 0) {
        instance = new EditRootLayer();
    }

    return instance;
}
