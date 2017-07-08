#ifndef LABEL_VIEW_HPP
#define LABEL_VIEW_HPP

#include <SDL_ttf.h>
#include "../uitk/RectangleView.hpp"

class LabelView : public RectangleView {
public:
    LabelView(Model* model, const SDL_Rect &rect);
    virtual ~LabelView();

    virtual void draw(SDL_Renderer* renderer) override;

    virtual std::string getText();
    virtual uint32_t getFontSize() const;
    virtual uint32_t getTextColor() const;

    virtual void setTextColor(uint32_t color);
    virtual void setText(std::string text);
    virtual void setMarginHorizontal(uint16_t marginHorizontal);
    virtual void setMarginVertical(uint16_t marginVertical);
    virtual void setIcon(Animation* animation);
    virtual void setFontPath(std::string fontPath);
    virtual void setFontSize(uint32_t fontSize);

    virtual void setHeight(uint16_t newHeight) override;
    virtual void setWidth(uint16_t newWidth) override;
    virtual void setX(uint16_t newX) override;
    virtual void setY(uint16_t newY) override;

private:
    TTF_Font* getFont(uint8_t size) const;
    SDL_Texture* makeTextTexture(SDL_Renderer *renderer) const;
    uint32_t chooseFontSizeToFit() const;

    std::string text;
    uint32_t textColor;
    uint16_t horizontalMargin;
    uint16_t verticalMargin;
    Animation* icon;
    std::string fontPath;
    bool textDirty;
    SDL_Texture* textTexture;
    uint32_t fontSize;
};

#endif
