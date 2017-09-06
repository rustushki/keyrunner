#ifndef TIMER_VIEW_HPP
#define TIMER_VIEW_HPP

#include "../uitk/LabelView.hpp"
#include "../model/PlayBoardModel.hpp"

class TimerView : public LabelView {
public:
    TimerView(PlayBoardModel* model, const SDL_Rect &rect);
    PlayBoardModel* getModel() const override;
    std::string getText() override;
};

#endif
