#ifndef LAYER_HPP
#define LAYER_HPP

#include <vector>
#include <SDL/SDL.h>

class Layer {

public:
    virtual void draw(SDL_Surface* dst);

    virtual SDL_Rect getRect() const = 0;

    virtual void update();

    virtual void onClick(uint16_t x, uint16_t y);

protected:
    void addLayer(Layer* subLayer);

    bool contains(uint16_t x, uint16_t y);

private:
    std::vector<Layer*> subLayers;

};

#endif//LAYER_HPP
