#ifndef PLAYROOTLAYER_HPP
#define PLAYROOTLAYER_HPP

#include "RootLayer.hpp"

class PlayRootLayer : public RootLayer {
public:

    static PlayRootLayer* GetInstance();

private:

    static PlayRootLayer* instance;

    PlayRootLayer();

};

#endif//PLAYROOTLAYER_HPP

