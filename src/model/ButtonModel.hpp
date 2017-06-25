#ifndef BUTTON_MODEL_HPP
#define BUTTON_MODEL_HPP

#include <string>

class ButtonModel {
public:
    ButtonModel(std::string text);
    std::string getText();
    void setClicked(bool clicked);
    bool isClicked();

private:
    std::string text;
    bool clicked;
};

#endif
