#ifndef EDITINFOBARLAYER_HPP
#define EDITINFOBARLAYER_HPP
#include <SDL/SDL.h>

#include "ButtonLayer.hpp"
#include "InfoBarLayer.hpp"

class EditInfoBarLayer : public InfoBarLayer {

public:
    static EditInfoBarLayer* GetInstance();
    ~EditInfoBarLayer();

private:
    EditInfoBarLayer();

    static EditInfoBarLayer* instance;

    ButtonLayer* exitBl;
    ButtonLayer* saveBl;

    void onExitClick();
    void onSaveClick();
};

#endif//EDITINFOBARLAYER_HPP
