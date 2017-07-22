#ifndef BUTTON_VIEW_HPP
#define BUTTON_VIEW_HPP

#include <SDL_ttf.h>
#include "../uitk/LabelView.hpp"

class ButtonView : public LabelView {
public:
    ButtonView(Model* model, const SDL_Rect &rect);
    ~ButtonView() override;
    void draw(SDL_Renderer* renderer) override;
    void onMouseDown(SDL_Event event) final;
    void onMouseUp(SDL_Event event) final;
    void onMouseHover(SDL_Event event) final;

private:
    bool isDepressed;
};

#endif