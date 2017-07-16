#ifndef MENU_VIEW_HPP
#define MENU_VIEW_HPP

#include <map>
#include "../uitk/RectangleView.hpp"
#include "../uitk/ButtonView.hpp"

class MenuView : public RectangleView {
public:
    MenuView(Model *model, const SDL_Rect &rect);
    virtual void draw(SDL_Renderer* renderer) override;
    virtual void onMouseHover(SDL_Event event) override;

    void addOption(std::string optionText, const std::function<void(SDL_Event)> &callBack);
    void setOptionBackgroundColor(uint32_t color);
    void setOptionTextColor(uint32_t color);
    void setOptionCursorTextColor(uint32_t color);
    void setCursorIndex(uint16_t index);
    void incrementCursor();
    void decrementCursor();
    uint16_t getCursorIndex() const;

private:
    std::vector<ButtonView*> buttons;
    uint32_t optionBackgroundColor;
    uint32_t optionTextColor;
    uint16_t cursorIndex;
    uint32_t cursorTextColor;

    void sizeButtons();
    uint32_t getOptionBackgroundColor();
    uint32_t getOptionTextColor();
};

#endif
