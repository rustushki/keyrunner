#ifndef IMAGE_VIEW_HPP
#define IMAGE_VIEW_HPP

#include "RectangleView.hpp"
#include "../view/AnimationType.hpp"

class ImageView : public RectangleView {
public:
    ImageView(Model *model, const SDL_Rect &rect, AnimationType animationType);
    virtual ~ImageView() override;
    void draw(SDL_Renderer* renderer) override;

private:
    Animation* animation;
};

#endif
