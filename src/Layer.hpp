#ifndef LAYER_HPP
#define LAYER_HPP

#include <functional>
#include <vector>
#include <SDL/SDL.h>

class Layer {

public:
    Layer();

    virtual void draw(SDL_Surface* dst);

    virtual SDL_Rect getRect() const = 0;

    virtual void update();

    virtual void onClick(uint16_t x, uint16_t y);

    void setOnClick(std::function<void()> cb);

protected:
    std::function<void()> getOnClickCb() const;

    void addLayer(Layer* subLayer);

    bool contains(uint16_t x, uint16_t y);

private:
    std::vector<Layer*> subLayers;

    std::function<void()> onClickCb;

};

#endif//LAYER_HPP
