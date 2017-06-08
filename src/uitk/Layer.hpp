#ifndef LAYER_HPP
#define LAYER_HPP

#include <functional>
#include <vector>
#include <SDL2/SDL.h>

class Layer {

public:
    Layer();

    virtual void draw(SDL_Renderer* renderer);

    virtual SDL_Rect getRect() const = 0;

    virtual void update();

    virtual void onEnter();

    virtual void onKeyDown(SDL_Keycode key);

    void setOnEnter(std::function<void()> cb);

    bool isVisible();

    virtual void show();

    virtual void hide();

    bool isSelected() const;

    void setSelected();

    static Layer* getSelectedLayer();

    Layer* getParent() const;

protected:
    std::function<void()> getOnEnterCb() const;

    void addLayer(Layer* subLayer);

    Layer* getChild(int i);

    int getSelectedChildIndex();

    uint16_t getChildCount();

    virtual void onSelected();

    void setParent(Layer* parent);

private:
    bool contains(uint16_t x, uint16_t y);

    std::vector<Layer*> subLayers;

    std::function<void()> onEnterCb;

    bool visible;

    static Layer* selectedLayer;

    Layer* parent;
};

#endif
