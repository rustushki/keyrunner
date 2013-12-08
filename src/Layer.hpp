#ifndef LAYER_HPP
#define LAYER_HPP

#include <vector>
#include <SDL/SDL.h>

class Layer {

public:
    virtual void draw(SDL_Surface* dst);

    virtual SDL_Rect getRect() const = 0;

    virtual void update() = 0;

protected:
    void addLayer(Layer* subLayer);

private:
    std::vector<Layer*> subLayers;

};

#endif//LAYER_HPP
