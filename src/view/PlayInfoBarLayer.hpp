#ifndef PLAYINFOBARLAYER_HPP
#define PLAYINFOBARLAYER_HPP
#include <SDL2/SDL.h>

#include "InfoBarLayer.hpp"

extern KeyRunner keyRunner;

class PlayInfoBarLayer : public InfoBarLayer {

public:
    static PlayInfoBarLayer* GetInstance();
    ~PlayInfoBarLayer();

    virtual void draw(SDL_Renderer* renderer, SDL_Texture* screen);

protected:
    TTF_Font* getFont() const;
    void drawText(SDL_Renderer* renderer, SDL_Texture* destination, std::string s, InfoBarPos position) const;
    void drawLevel(SDL_Renderer* renderer, SDL_Texture* dst, uint16_t level) const;
    void drawTimer(SDL_Renderer* renderer, SDL_Texture* destination) const;

private:
    PlayInfoBarLayer();

    static PlayInfoBarLayer* instance;
};

#endif//PLAYINFOBARLAYER_HPP
