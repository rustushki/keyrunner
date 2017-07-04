#ifndef BASE_VIEW_HPP
#define BASE_VIEW_HPP

#include "../uitk/View.hpp"

class BaseView : public View {
public:
    BaseView(Model* model, SDL_Rect rect);
    virtual ~BaseView() override;

    virtual Model* getModel() const override;
    virtual void draw(SDL_Renderer* renderer) override = 0;
    virtual SDL_Rect getRect() const override;
    virtual void show() override;
    virtual void hide() override;
    virtual bool isVisible() const override;
    virtual const std::vector<Animation*> getAnimations() const override;
    virtual void setOnClickCallback(const std::function <void ()>& newCallBack) override final;
    virtual void onClick() const override final;
    virtual bool isPressed() const override final;
    virtual void setHeight(uint16_t newHeight) override;
    virtual void setWidth(uint16_t newWidth) override;
    virtual void setX(uint16_t newX) override;
    virtual void setY(uint16_t newY) override;

protected:
    void addAnimation(Animation*);

private:
    Model *model;
    SDL_Rect rect;
    bool visible;
    std::vector<Animation *> animations;
    std::function <void ()> onClickCallBack = [] () {};
};


#endif
