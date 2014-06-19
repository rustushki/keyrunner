#ifndef EDITINFOBARLAYER_HPP
#define EDITINFOBARLAYER_HPP
#include <SDL/SDL.h>

#include "ButtonLayer.hpp"
#include "InfoBarLayer.hpp"
#include "TileSelectorLayer.hpp"

class EditInfoBarLayer : public InfoBarLayer {

public:
    static EditInfoBarLayer* GetInstance();
    ~EditInfoBarLayer();

private:
    EditInfoBarLayer();

    static EditInfoBarLayer* instance;

    void onKeyDown(SDLKey key);

    ButtonLayer* exitBl;
    ButtonLayer* saveBl;
    ButtonLayer* tileBl;

    void onExitEnter();
    void onSaveEnter();
    void onTileEnter();

    TileSelectorLayer* tsl;
};

#endif//EDITINFOBARLAYER_HPP
