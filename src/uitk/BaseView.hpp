#ifndef BASE_VIEW_HPP
#define BASE_VIEW_HPP

#include "../uitk/View.hpp"

class BaseView : public View {
public:
    BaseView(Model* model, SDL_Rect rect);
    virtual ~BaseView();

    virtual Model* getModel() const;
    virtual void draw(SDL_Renderer* renderer) = 0;
    virtual SDL_Rect getRect() const;
    virtual void show();
    virtual void hide();
    virtual bool isVisible() const;
    virtual const std::vector<Animation*> getAnimations() const;

protected:
    void addAnimation(Animation*);

private:
    Model *model;
    SDL_Rect rect;
    bool visible;
    std::vector<Animation *> animations;
};


#endif
