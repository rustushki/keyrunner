#ifndef INFOBARLAYER_HPP
#define INFOBARLAYER_HPP
#include <SDL/SDL_ttf.h>
#include "InfoBarPos.hpp"
#include "KeyRunner.hpp"
#include "Layer.hpp"

class InfoBarLayer : public Layer {

public:
    virtual void draw(SDL_Surface* screen);
    virtual SDL_Rect getRect() const;

protected:
    InfoBarLayer();

};

#endif//INFOBARLAYER_HPP
