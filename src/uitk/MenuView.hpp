#ifndef MENU_VIEW_HPP
#define MENU_VIEW_HPP

#include <map>
#include "../uitk/ButtonView.hpp"
#include "../uitk/ImageView.hpp"

class MenuView : public RectangleView {
public:
    MenuView(Model* model, const SDL_Rect &rect, AnimationFactory* animationFactory);
    void draw(SDL_Renderer* renderer) override;
    void onMouseHover(SDL_Event event) override;
    void onMouseUp(SDL_Event event) override;
    void onKeyUp(SDL_Event event) override;

    void setOptionFontSize(uint8_t optionFontSize);
    void setVisibleOptionCount(uint16_t count);
    uint16_t getVisibleOptionCount() const;
    void addOption(std::string optionText, const std::function<void(SDL_Event)> &callBack);
    void setOptionBackgroundColor(uint32_t color);
    void setOptionTextColor(uint32_t color);
    void setOptionCursorTextColor(uint32_t color);
    void setCursorIndex(uint16_t index);
    void incrementCursor();
    void decrementCursor();
    uint16_t getCursorIndex() const;
    uint8_t getOptionFontSize() const;

private:
    ImageView* upArrowView;
    ImageView* downArrowView;
    std::vector<ButtonView*> buttons;
    uint32_t optionBackgroundColor;
    uint32_t optionTextColor;
    uint16_t windowTopIndex;
    uint16_t cursorIndex;
    uint32_t cursorTextColor;
    uint16_t visibleOptionCount;
    uint8_t optionFontSize;
    AnimationFactory* animationFactory;

    void sizeButtons();
    uint32_t getOptionBackgroundColor();
    uint32_t getOptionTextColor();
    void createArrows();
    bool cantScrollDown() const;
    bool cantScrollUp() const;
    bool isScrollingEverNecessary() const;
    uint16_t getWindowSize() const;
    uint16_t getWindowTopIndex() const;
    void setWindowTopIndex(uint16_t index);
    void incrementWindowTopIndex();
    void decrementWindowTopIndex();
    bool isCursorOutsideWindow();
    void setAnimationFactory(AnimationFactory* animationFactory);
    AnimationFactory* getAnimationFactory() const;

};

#endif
