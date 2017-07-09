#ifndef TIMER_VIEW_HPP
#define TIMER_VIEW_HPP

#include "../uitk/LabelView.hpp"
#include "../model/PlayBoardModel.hpp"

class TimerView : public LabelView {
public:
    TimerView(PlayBoardModel* model, const SDL_Rect &rect);
    virtual PlayBoardModel* getModel() const override;
    virtual std::string getText() override;
};

#endif
