#include <algorithm>
#include "../controller/Display.hpp"

/**
 * Constructor.
 * <p>
 * Stores the SDL_Window* and SDL_Renderer* for the display.
 * @param window
 * @param renderer
 */
Display::Display(SDL_Window *window, SDL_Renderer *renderer) {
    this->window = window;
    this->renderer = renderer;
}

/**
 * De-constructor.
 */
Display::~Display() {
    this->reset();
}

/**
 * Re-draw each View onto the back buffer, then present it.
 */
void Display::draw() const {
    // Clear the back frame to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);

    // Redraw each View
    for (auto viewName : viewNameStack) {
        View* view = getViewByName(viewName);
        if (view->isVisible()) {
            view->draw(renderer);
        }
    }

    // Present the back frame
    SDL_RenderPresent(renderer);
}

/**
 * Add a new view to the Display with a given name.
 * <p>
 * The added view will be at the top of the stack and will cause the removal and freeing of any view that already has
 * the provided name.
 * @param name
 * @param view
 */
void Display::addView(std::string name, View *view) {
    deleteViewIfExists(name);
    viewMap[name] = view;
    viewNameStack.push_back(name);
}

/**
 * Delete a View by its given name (if that view exists).
 * @param name
 */
void Display::deleteViewIfExists(std::string name) {
    // Erase the name from the viewNameStack
    auto positionToRemove = std::remove(viewNameStack.begin(), viewNameStack.end(), name);
    viewNameStack.erase(positionToRemove, viewNameStack.end());

    // Erase (and free) the view from the viewMap
    auto viewIterator = viewMap.find(name);
    if (viewIterator != viewMap.end()) {
        delete viewIterator->second;
        viewMap.erase(viewIterator);
    }
}

/**
 * Get the height of the Display's window.
 * @return height of window
 */
uint16_t Display::getHeight() const {
    int height;
    SDL_GetWindowSize(window, nullptr, &height);
    return (uint16_t) height;
}

/**
 * Get the width of the Display's window.
 * @return width of window
 */
uint16_t Display::getWidth() const {
    int width;
    SDL_GetWindowSize(window, &width, nullptr);
    return (uint16_t) width;
}

/**
 * Iterate over the Views currently displayed and advance each of their Animations.
 */
void Display::advanceAnimations() {
    for (auto viewName : viewNameStack) {
        View* view = getViewByName(viewName);
        for (Animation* animation : view->getAnimations()) {
            animation->advance();
        }
    }
}

/**
 * Given a coordinate pair, return view underneath it, or nullptr if no view is underneath.
 * <p>
 * If views are stacked on top of each other, the uppermost view will be considered clicked. Any beneath will not be
 * returned.
 * @param x x coordinate
 * @param y y coordinate
 * @return View*
 */
View *Display::getViewUnderCoordinate(uint32_t x, uint32_t y) const {
    View* matchingView = nullptr;

    for (auto viewNameIterator = viewNameStack.rbegin(); viewNameIterator != viewNameStack.rend(); viewNameIterator++) {
        View* view = getViewByName(*viewNameIterator);
        if (view->containsPoint(x, y)) {
            matchingView = view;
            break;
        }
    }

    return matchingView;
}

/**
 * Find the View on the display that has the given name and return it.
 * @param name
 * @return View*
 */
View* Display::getViewByName(std::string name) const {
    View* matchingView = nullptr;
    for (auto viewPair : viewMap) {
        std::string viewName = viewPair.first;
        if (viewName == name) {
            matchingView = viewPair.second;
        }
    }

    return matchingView;
}

/**
 * Set the view which should be considered in focus.
 * @param viewName
 */
void Display::setFocus(std::string viewName) {
    this->focusedViewName = viewName;
}

/**
 * Get the view which should be considered in focus.
 * @return string
 */
std::string Display::getFocus() const {
    return this->focusedViewName;
}

/**
 * Reset the display back to a clean state.
 * <p>
 * This will un-name all views, delete the views (thus freeing memory), clear the internal stack of views, and remove
 * focus from any views that were in focus before.
 */
void Display::reset() {
    // Free all Views
    for (auto viewPair : viewMap) {
        delete viewPair.second;
    }

    viewMap.clear();
    viewNameStack.clear();
    setFocus("");
}
