#ifndef LAYER_HPP
#define LAYER_HPP

#include <SDL/SDL.h>

class Layer {

public:
    virtual void draw(SDL_Surface* dst) = 0;

};

#endif//LAYER_HPP
