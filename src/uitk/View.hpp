#ifndef VIEW_HPP
#define VIEW_HPP

#include <SDL_system.h>
#include "../model/PlayModel.hpp"
#include "../uitk/Animation.hpp"

class View {
public:
    virtual ~View() {}
    virtual PlayModel* getModel() const = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
    virtual SDL_Rect getRect() const = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual bool isVisible() const = 0;
    virtual const std::vector<Animation*> getAnimations() const = 0;
};

#endif
