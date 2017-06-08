#ifndef INFO_BAR_LAYER_HPP
#define INFO_BAR_LAYER_HPP
#include <SDL2/SDL_ttf.h>
#include "InfoBarPos.hpp"
#include "../controller/KeyRunner.hpp"
#include "../uitk/Layer.hpp"

class InfoBarLayer : public Layer {

public:
    virtual void draw(SDL_Renderer* renderer);
    virtual SDL_Rect getRect() const;

protected:
    InfoBarLayer();

};

#endif
