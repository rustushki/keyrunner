#ifndef ROOTLAYER_HPP
#define ROOTLAYER_HPP

#include "../uitk/Layer.hpp"

class InfoBarLayer;

class RootLayer : public Layer {
public:

    virtual void draw(SDL_Renderer* renderer, SDL_Texture* destination);

    virtual SDL_Rect getRect() const;

protected:
    RootLayer(InfoBarLayer* ibl);

private:

    InfoBarLayer* ibl;

};

#endif//ROOTLAYER_HPP
