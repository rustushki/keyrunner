#ifndef RECTANGLE_VIEW_HPP
#define RECTANGLE_VIEW_HPP

#include "../uitk/BaseView.hpp"

class RectangleView : public BaseView {
public:
    RectangleView(Model* model, const SDL_Rect &rect);
    ~RectangleView() override = default;
    void draw(SDL_Renderer* renderer) override;
    void setColor(uint32_t newColor);
    uint32_t getColor() const;

private:
    uint32_t color;
};

#endif
