#ifndef MENU_VIEW_HPP
#define MENU_VIEW_HPP

#include "RectangleView.hpp"

class MenuView : public RectangleView {
public:
    MenuView(Model *model, const SDL_Rect &rect);
};

#endif
