#ifndef BUTTON_VIEW_HPP
#define BUTTON_VIEW_HPP

#include <SDL_ttf.h>
#include "../uitk/LabelView.hpp"

class ButtonView : public LabelView {
public:
    ButtonView(Model* model, const SDL_Rect &rect);
    virtual ~ButtonView() override;
    virtual void draw(SDL_Renderer* renderer) override;

};

#endif