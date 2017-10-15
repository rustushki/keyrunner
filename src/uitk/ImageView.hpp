#ifndef IMAGE_VIEW_HPP
#define IMAGE_VIEW_HPP

#include "../uitk/HorizontalAlignment.hpp"
#include "../uitk/RectangleView.hpp"
#include "../uitk/VerticalAlignment.hpp"
#include "../view/AnimationFactory.hpp"
#include "../view/AnimationType.hpp"

class ImageView : public RectangleView {
public:
    ImageView(Model* model, const SDL_Rect &rect, AnimationFactory* animationFactory, AnimationType animationType);
    ~ImageView() override;
    void draw(SDL_Renderer* renderer) override;
    void setHorizontalAlignment(HorizontalAlignment alignment);
    void setVerticalAlignment(VerticalAlignment alignment);
    HorizontalAlignment getHorizontalAlignment() const;
    VerticalAlignment getVerticalAlignment() const;

private:
    Animation* animation;
    HorizontalAlignment horizontalAlignment;
    VerticalAlignment verticalAlignment;

    int getHorizontalOffset() const;
    int getVerticalOffset() const;
};

#endif
