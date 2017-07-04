#ifndef TIMER_VIEW_HPP
#define TIMER_VIEW_HPP

#include "../uitk/LabelView.hpp"
#include "../model/PlayModel.hpp"

class TimerView : public LabelView {
public:
    TimerView(PlayModel* model, const SDL_Rect &rect);
    virtual PlayModel* getModel() const;
    virtual std::string getText() override;
};

#endif
