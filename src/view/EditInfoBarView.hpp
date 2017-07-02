#ifndef EDIT_INFO_BAR_VIEW_HPP
#define EDIT_INFO_BAR_VIEW_HPP

#include "../uitk/BaseView.hpp"

class EditInfoBarView : public BaseView {
public:
    EditInfoBarView(const SDL_Rect &rect);
    virtual ~EditInfoBarView() override;
    void draw(SDL_Renderer* renderer) override;

};

#endif
