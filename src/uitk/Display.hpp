#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <string>
#include <SDL_system.h>
#include <map>
#include "../uitk/View.hpp"

class Display {
public:
    Display(SDL_Window* window, SDL_Renderer* renderer);
    ~Display();

    void draw() const;
    void addView(std::string name, View* view);
    void deleteViewIfExists(std::string name);
    uint16_t getHeight() const;
    uint16_t getWidth() const;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::map<std::string, View*> viewMap;
};

#endif
