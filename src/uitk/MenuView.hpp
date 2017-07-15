#ifndef MENU_VIEW_HPP
#define MENU_VIEW_HPP

#include <map>
#include "../uitk/RectangleView.hpp"
#include "../uitk/ButtonView.hpp"

class MenuView : public RectangleView {
public:
    MenuView(Model *model, const SDL_Rect &rect);
    virtual void draw(SDL_Renderer* renderer) override;

    void addOption(std::string optionText, const std::function<void(SDL_Event)> &callBack);
    void setOptionBackgroundColor(uint32_t color);
    void setOptionTextColor(uint32_t color);

private:
    std::vector<ButtonView*> buttons;
    uint32_t optionBackgroundColor;
    uint32_t optionTextColor;

    void sizeButtons();
    uint32_t getOptionBackgroundColor();
    uint32_t getOptionTextColor();
};

#endif
