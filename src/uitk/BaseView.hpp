#ifndef BASE_VIEW_HPP
#define BASE_VIEW_HPP

#include "../uitk/View.hpp"

class BaseView : public View {
public:
    BaseView(Model* model, SDL_Rect rect);
    virtual ~BaseView() override;

    virtual Model* getModel() const override;
    virtual void draw(SDL_Renderer* renderer) override = 0;
    virtual void show() override;
    virtual void hide() override;
    virtual bool isVisible() const override;
    virtual const std::vector<Animation*> getAnimations() const override;
    virtual void setOnMouseUpCallback(const std::function<void(SDL_Event)> &newCallBack) override final;
    virtual void setOnMouseDownCallback(const std::function<void(SDL_Event)> &newCallBack) override final;
    virtual void setOnMouseHoverCallback(const std::function<void(SDL_Event)> &newCallBack) override final;
    virtual void onMouseUp(SDL_Event event) const override final;
    virtual void onMouseDown(SDL_Event event) const override final;
    virtual void onMouseHover(SDL_Event event) const override final;
    virtual bool isPressed() const override final;
    virtual void setHeight(uint16_t newHeight) override;
    virtual void setWidth(uint16_t newWidth) override;
    virtual void setX(uint16_t newX) override;
    virtual void setY(uint16_t newY) override;
    virtual uint16_t getHeight() const override;
    virtual uint16_t getWidth() const override;
    virtual uint16_t getX() const override;
    virtual uint16_t getY() const override;
    virtual bool containsPoint(uint32_t x, uint32_t y) const override;

protected:
    void addAnimation(Animation*);

private:
    Model *model;
    SDL_Rect rect;
    bool visible;
    std::vector<Animation *> animations;
    std::function <void (SDL_Event)> onMouseUpCallBack = [] (SDL_Event event) {};
    std::function <void (SDL_Event)> onMouseDownCallBack = [] (SDL_Event event) {};
    std::function <void (SDL_Event)> onMouseHoverCallBack = [] (SDL_Event event) {};
};


#endif
