#include "../uitk/Display.hpp"

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
 * <p>
 * Frees all the Views that are currently managed.
 */
Display::~Display() {
    // Free all Views
    for (auto viewPair : viewMap) {
        delete viewPair.second;
    }
}

/**
 * Re-draw each View onto the back buffer, then present it.
 */
void Display::draw() const {
    // Clear the back frame to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);

    // Redraw each View
    for (auto viewPair : viewMap) {
        View* view = viewPair.second;
        if (view->isVisible()) {
            view->draw(renderer);
        }
    }

    // Present the back frame
    SDL_RenderPresent(renderer);
}

/**
 * Add a new View to the Display with a given name.
 * @param name
 * @param view
 */
void Display::addView(std::string name, View *view) {
    deleteViewIfExists(name);
    viewMap[name] = view;
}

/**
 * Delete a View by its given name (if that view exists).
 * @param name
 */
void Display::deleteViewIfExists(std::string name) {
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
    for (auto viewPair : viewMap) {
        View* view = viewPair.second;
        for (Animation* animation : view->getAnimations()) {
            animation->advance();
        }
    }
}
