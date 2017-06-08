#ifndef ROOT_LAYER_HPP
#define ROOT_LAYER_HPP

#include "../uitk/Layer.hpp"

class InfoBarLayer;

class RootLayer : public Layer {
public:

    virtual void draw(SDL_Renderer* renderer);

    virtual SDL_Rect getRect() const;

protected:
    RootLayer(InfoBarLayer* ibl);

private:

    InfoBarLayer* ibl;

};

#endif
