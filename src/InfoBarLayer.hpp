#ifndef INFOBAR_HPP
#define INFOBAR_HPP
#include <SDL/SDL_ttf.h>
#include "InfoBarPos.hpp"
#include "KeyRunner.hpp"
#include "Layer.hpp"
#include "Level.hpp"

class InfoBarLayer : public Layer {

public:
    static InfoBarLayer* GetInstance();
    ~InfoBarLayer();

    virtual void draw(SDL_Surface* screen);
    int getHeight() const;
    int getWidth() const;
    int getX() const;
    int getY() const;

private:
    InfoBarLayer();

    TTF_Font* getFont() const;
    void drawText(SDL_Surface* dst, std::string s, InfoBarPos position) const;
    void drawLevel(SDL_Surface* dst, uint16_t level) const;
    void drawTimer(SDL_Surface* dst) const;

    static InfoBarLayer* instance;

    SDL_Surface* ibSrf;

};

#endif//INFOBAR_HPP
