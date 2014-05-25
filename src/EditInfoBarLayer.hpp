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

    ButtonLayer* exitBl;
    ButtonLayer* saveBl;
    ButtonLayer* tileBl;

    void onExitClick();
    void onSaveClick();
    void onTileClick();

    TileSelectorLayer* tsl;
};

#endif//EDITINFOBARLAYER_HPP
