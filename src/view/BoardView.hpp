#ifndef BOARD_VIEW_HPP
#define BOARD_VIEW_HPP

#include "../uitk/BaseView.hpp"
#include "../uitk/Animation.hpp"

class BoardView : public BaseView {
public:
    BoardView(PlayModel* model, SDL_Rect rect);
    virtual ~BoardView();
    void draw(SDL_Renderer* renderer);

private:
    Animation *keyAnimation;
    Animation* playerAnimation;
};

#endif
