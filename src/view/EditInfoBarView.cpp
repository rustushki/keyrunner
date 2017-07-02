#include "../view/EditInfoBarView.hpp"

/**
 * Constructor.
 * @param model
 * @param rect
 */
EditInfoBarView::EditInfoBarView(Model *model, const SDL_Rect &rect) : BaseView(model, rect) {}

/**
 * Destructor.
 */
EditInfoBarView::~EditInfoBarView() {

}

/**
 * Draws a black bar to the bottom of the screen.
 * @param renderer
 */
void EditInfoBarView::draw(SDL_Renderer *renderer) {
    SDL_Rect rect = getRect();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer, &rect);
}