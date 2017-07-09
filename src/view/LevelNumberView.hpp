#ifndef LEVEL_NUMBER_VIEW_HPP
#define LEVEL_NUMBER_VIEW_HPP

#include "../uitk/LabelView.hpp"
#include "../model/BoardModel.hpp"

class LevelNumberView : public LabelView {
public:
    LevelNumberView(BoardModel* model, const SDL_Rect &rect);
    virtual BoardModel* getModel() const override;
    virtual std::string getText() override;

};

#endif
