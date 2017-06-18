#ifndef PLAY_INFO_BAR_VIEW_HPP
#define PLAY_INFO_BAR_VIEW_HPP

#include <SDL_ttf.h>
#include "../uitk/View.hpp"
#include "../view/InfoBarPos.hpp"
#include "../uitk/BaseView.hpp"

class PlayInfoBarView : public BaseView {
public:
    PlayInfoBarView(PlayModel* model, SDL_Rect rect);
    void draw(SDL_Renderer* renderer);

private:
    TTF_Font* getFont() const;
    void drawText(SDL_Renderer* renderer, std::string text, InfoBarPos position) const;
    void drawBlackBar(SDL_Renderer* renderer) const;
    void drawLevel(SDL_Renderer *renderer, uint16_t level) const;
    void drawTimer(SDL_Renderer* renderer) const;
};

#endif
