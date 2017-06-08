#ifndef PLAY_ROOT_LAYER_HPP
#define PLAY_ROOT_LAYER_HPP

#include "RootLayer.hpp"

class PlayRootLayer : public RootLayer {
public:

    static PlayRootLayer* GetInstance();

private:

    static PlayRootLayer* instance;

    PlayRootLayer();

};

#endif
