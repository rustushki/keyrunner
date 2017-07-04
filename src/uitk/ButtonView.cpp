#include <sstream>
#include "../uitk/ButtonView.hpp"

/**
 * Constructor.
 * @param rect
 */
ButtonView::ButtonView(Model* model, const SDL_Rect &rect) : LabelView(model, rect) {}

/**
 * Destructor.
 */
ButtonView::~ButtonView() {
}

/**
 * Draw the Button to the screen.
 * <p>
 * Eventually, this could support on click and hover. Right now, it does nothing more than LabelView::draw().
 * @param renderer
 */
void ButtonView::draw(SDL_Renderer *renderer) {
    LabelView::draw(renderer);
}

