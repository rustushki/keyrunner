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

    virtual void onKeyDown(SDLKey key);

    void setOnClick(std::function<void()> cb);

    bool isVisible();

    virtual void show();

    virtual void hide();

    bool isSelected() const;

    void setSelected();

protected:
    std::function<void()> getOnClickCb() const;

    void addLayer(Layer* subLayer);

    Layer* getChild(int i);

    int getSelectedChildIndex();

    uint16_t getChildCount();

    virtual void onSelected();

    void setParent(Layer* parent);

private:
    bool contains(uint16_t x, uint16_t y);

    std::vector<Layer*> subLayers;

    std::function<void()> onClickCb;

    bool visible;

    static Layer* selectedLayer;

    Layer* parent;
};

#endif//LAYER_HPP
