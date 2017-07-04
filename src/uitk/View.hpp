#ifndef VIEW_HPP
#define VIEW_HPP

#include <SDL_system.h>
#include <functional>
#include "../uitk/Animation.hpp"
#include "../model/Model.hpp"

class View {
public:
    virtual ~View() {}
    virtual Model* getModel() const = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
    virtual SDL_Rect getRect() const = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual bool isVisible() const = 0;
    virtual const std::vector<Animation*> getAnimations() const = 0;
    virtual void setOnClickCallback(const std::function <void ()>& newCallBack) = 0;
    virtual void onClick() const = 0;
    virtual bool isPressed() const = 0;
    virtual void setHeight(uint16_t height) = 0;
    virtual void setWidth(uint16_t newWidth) = 0;
    virtual void setX(uint16_t pX) = 0;
    virtual void setY(uint16_t pY) = 0;
};

#endif
