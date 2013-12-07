#ifndef INFOBAR_HPP
#define INFOBAR_HPP
#include <SDL/SDL_ttf.h>
#include "InfoBarPos.hpp"
#include "KeyRunner.hpp"
#include "Level.hpp"

class InfoBar {

public:
    static InfoBar* GetInstance();
    ~InfoBar();

    SDL_Surface* getSurface(uint16_t level) const;
    int getHeight() const;
    int getWidth() const;
    int getX() const;
    int getY() const;

private:
    InfoBar();

    TTF_Font* getFont() const;
    void drawText(std::string s, InfoBarPos position) const;
    void drawLevel(uint16_t level) const;
    void drawTimer() const;

    static InfoBar* instance;

    SDL_Surface* ibSrf;

};

#endif//INFOBAR_HPP
