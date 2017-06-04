#ifndef PLAYINFOBARLAYER_HPP
#define PLAYINFOBARLAYER_HPP
#include <SDL/SDL.h>

#include "InfoBarLayer.hpp"

extern KeyRunner keyRunner;

class PlayInfoBarLayer : public InfoBarLayer {

public:
    static PlayInfoBarLayer* GetInstance();
    ~PlayInfoBarLayer();

    virtual void draw(SDL_Surface* screen);

protected:
    TTF_Font* getFont() const;
    void drawText(SDL_Surface* dst, std::string s, InfoBarPos position) const;
    void drawLevel(SDL_Surface* dst, uint16_t level) const;
    void drawTimer(SDL_Surface* dst) const;

private:
    PlayInfoBarLayer();

    static PlayInfoBarLayer* instance;
};

#endif//PLAYINFOBARLAYER_HPP
