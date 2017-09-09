#include <sstream>
#include "TimerView.hpp"

/**
 * Constructor.
 * @param model
 * @param rect
 */
TimerView::TimerView(PlayBoardModel* model, const SDL_Rect &rect) : LabelView(model, rect) {}

/**
 * Return the text that appears on this label.
 * <p>
 * In this case, the text will be the number of seconds left on the game clock (including 1/10's of seconds).
 * @return
 */
std::string TimerView::getText() {
    std::stringstream clockTime;
    clockTime << std::fixed;
    clockTime.precision(1);
    clockTime << (double) getModel()->getTimeClock() / (double) 1000;
    setText(clockTime.str());
    return LabelView::getText();
}

/**
 * Get the model to which this label is bound.
 * @return PlayModel*
 */
PlayBoardModel* TimerView::getModel() const {
    return (PlayBoardModel*) LabelView::getModel();
}
