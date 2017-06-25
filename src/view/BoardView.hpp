#ifndef BOARD_VIEW_HPP
#define BOARD_VIEW_HPP

#include <map>
#include "../uitk/BaseView.hpp"
#include "../uitk/Animation.hpp"
#include "../model/Board.hpp"

class BoardView : public BaseView {
public:
    BoardView(Board* model, SDL_Rect rect);
    virtual ~BoardView();
    void draw(SDL_Renderer* renderer);
    Board* getModel() const;

private:
    Animation* keyAnimation;
    Animation* playerAnimation;
    std::map<AnimationType, Animation*> preBuiltAnimations;

    void preBuildAnimations();
    void freeAnimations();

    Animation* conveyDownAnimation;
    Animation* conveyUpAnimation;
    Animation* conveyLeftAnimation;
    Animation* conveyRightAnimation;
};

#endif
