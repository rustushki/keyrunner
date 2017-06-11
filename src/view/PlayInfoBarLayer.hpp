#ifndef PLAY_INFO_BAR_LAYER_HPP
#define PLAY_INFO_BAR_LAYER_HPP
#include <SDL2/SDL.h>

#include "InfoBarLayer.hpp"

class PlayInfoBarLayer : public InfoBarLayer {

public:
    static PlayInfoBarLayer* GetInstance();
    ~PlayInfoBarLayer();

    virtual void draw(SDL_Renderer* renderer);

protected:
    TTF_Font* getFont() const;
    void drawText(SDL_Renderer* renderer, std::string s, InfoBarPos position) const;
    void drawLevel(SDL_Renderer* renderer, uint16_t level) const;
    void drawTimer(SDL_Renderer* renderer) const;

private:
    PlayInfoBarLayer();

    static PlayInfoBarLayer* instance;
};

#endif
