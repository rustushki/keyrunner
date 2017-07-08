#include <sstream>
#include "../uitk/ButtonView.hpp"

/**
 * Constructor.
 * @param rect
 */
ButtonView::ButtonView(Model* model, const SDL_Rect &rect) : LabelView(model, rect) {
    isDepressed = false;
}

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

/**
 * Default behavior, but also inverts the text and background colors.
 * @param event
 */
void ButtonView::onMouseDown(SDL_Event event) {
    if (!isDepressed) {
        this->setColor(~this->getColor());
        this->setTextColor(~this->getTextColor());
        isDepressed = true;
    }
    BaseView::onMouseDown(event);
}

/**
 * Default behavior, but if the button is depressed, it will un-depress it by inverting the text and background colors
 * back to their originals.
 * @param event
 */
void ButtonView::onMouseUp(SDL_Event event) {
    if (isDepressed) {
        this->setColor(~this->getColor());
        this->setTextColor(~this->getTextColor());
        isDepressed = false;
    }
    BaseView::onMouseUp(event);
}

/**
 * Default behavior, but if the button is depressed, it will un-depress it by inverting the text and background colors
 * back to their originals.
 * @param event
 */
void ButtonView::onMouseHover(SDL_Event event) {
    if (isDepressed) {
        this->setColor(~this->getColor());
        this->setTextColor(~this->getTextColor());
        isDepressed = false;
    }
    BaseView::onMouseHover(event);
}

