#ifndef RECTANGLE_VIEW_HPP
#define RECTANGLE_VIEW_HPP

#include "../uitk/BaseView.hpp"

class RectangleView : public BaseView {
public:
    RectangleView(const SDL_Rect &rect);
    virtual ~RectangleView() override;
    void draw(SDL_Renderer* renderer) override;
    void setColor(uint32_t newColor);

private:
    uint32_t color;
};

#endif
