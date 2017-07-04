#include <sstream>
#include "LevelNumberView.hpp"

/**
 * Constructor.
 * @param model
 * @param rect
 */
LevelNumberView::LevelNumberView(BoardModel *model, const SDL_Rect &rect) : LabelView(model, rect) {}

/**
 * Gets the text of this label.
 * <p>
 * The value will be "Level X" where X is the level number.
 * @return std::string
 */
std::string LevelNumberView::getText() {
    std::stringstream text;
    text << "Level " << (uint32_t) getModel()->getLevelNum();
    setText(text.str());
    return LabelView::getText();
}

/**
 * Fetch the BoardModel to which this label is bound.
 * @return
 */
BoardModel* LevelNumberView::getModel() const {
    return (BoardModel*) LabelView::getModel();
}
