#ifndef EDITINFOBARLAYER_HPP
#define EDITINFOBARLAYER_HPP
#include <SDL2/SDL.h>

#include "InfoBarLayer.hpp"
#include "TileSelectorLayer.hpp"
#include "../uitk/ButtonLayer.hpp"

extern KeyRunner keyRunner;

class EditInfoBarLayer : public InfoBarLayer {

public:
    static EditInfoBarLayer* GetInstance();
    ~EditInfoBarLayer();

private:
    EditInfoBarLayer();

    static EditInfoBarLayer* instance;

    void onKeyDown(SDL_Keycode key);

    ButtonLayer* exitBl;
    ButtonLayer* saveBl;
    ButtonLayer* tileBl;

    void onExitEnter();
    void onSaveEnter();
    void onTileEnter();

    TileSelectorLayer* tsl;
};

#endif//EDITINFOBARLAYER_HPP
