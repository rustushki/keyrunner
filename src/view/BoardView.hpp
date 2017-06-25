#ifndef BOARD_VIEW_HPP
#define BOARD_VIEW_HPP

#include <map>
#include "../uitk/BaseView.hpp"
#include "../uitk/Animation.hpp"
#include "../model/BoardModel.hpp"

class BoardView : public BaseView {
public:
    BoardView(BoardModel* model, SDL_Rect rect);
    virtual ~BoardView();
    void draw(SDL_Renderer* renderer);
    BoardModel* getModel() const;

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
