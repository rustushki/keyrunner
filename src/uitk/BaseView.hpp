#ifndef BASE_VIEW_HPP
#define BASE_VIEW_HPP

#include "../uitk/View.hpp"

class BaseView : public View {
public:
    BaseView(Model* model, SDL_Rect rect);
    ~BaseView() override;

    Model* getModel() const override;
    void draw(SDL_Renderer* renderer) override = 0;
    void show() override;
    void hide() override;
    bool isVisible() const override;
    const std::vector<Animation*> getAnimations() const override;
    void setOnMouseUpCallback(const std::function<void(SDL_Event)> &newCallBack) final;
    void setOnMouseDownCallback(const std::function<void(SDL_Event)> &newCallBack) final;
    void setOnMouseHoverCallback(const std::function<void(SDL_Event)> &newCallBack) final;
    void setOnKeyUpCallback(const std::function<void(SDL_Event)> &newCallBack) final;
    void onMouseUp(SDL_Event event) override;
    void onMouseDown(SDL_Event event) override;
    void onMouseHover(SDL_Event event) override;
    void onKeyUp(SDL_Event event) override;
    void setHeight(uint16_t newHeight) override;
    void setWidth(uint16_t newWidth) override;
    void setX(uint16_t newX) override;
    void setY(uint16_t newY) override;
    uint16_t getHeight() const override;
    uint16_t getWidth() const override;
    uint16_t getX() const override;
    uint16_t getY() const override;
    bool containsPoint(uint32_t x, uint32_t y) const override;

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
    std::function <void (SDL_Event)> onMouseKeyUpCallBack = [] (SDL_Event event) {};
};


#endif
