#ifndef PLAYINFOBARLAYER_HPP
#define PLAYINFOBARLAYER_HPP
#include <SDL/SDL.h>

#include "InfoBarLayer.hpp"

class PlayInfoBarLayer : public InfoBarLayer {

public:
    static PlayInfoBarLayer* GetInstance();
    ~PlayInfoBarLayer();

private:
    PlayInfoBarLayer();

    static PlayInfoBarLayer* instance;
};

#endif//PLAYINFOBARLAYER_HPP
