#ifndef BOARD_VIEW_HPP
#define BOARD_VIEW_HPP

#include <map>
#include "../uitk/BaseView.hpp"
#include "../uitk/Animation.hpp"
#include "../model/AnimationTypeFactory.hpp"
#include "../model/BoardModel.hpp"
#include "../view/AnimationFactory.hpp"

class BoardView : public BaseView {
public:
    BoardView(BoardModel* model, SDL_Rect rect, AnimationFactory* animationFactory);
    ~BoardView() override;
    void draw(SDL_Renderer* renderer) override;
    BoardModel* getModel() const override;
    uint32_t getTileWidth() const;
    uint32_t getTileHeight() const;

protected:
    std::map<AnimationType, Animation*> getPreBuiltAnimations() const;

private:
    Animation* keyAnimation;
    Animation* playerAnimation;
    std::map<AnimationType, Animation*> preBuiltAnimations;
    Animation* conveyDownAnimation;
    Animation* conveyUpAnimation;
    Animation* conveyLeftAnimation;
    Animation* conveyRightAnimation;
    AnimationFactory* animationFactory;

    void preBuildAnimations();
    void freeAnimations();
    AnimationFactory* getAnimationFactory() const;
    void setAnimationFactory(AnimationFactory* animationFactory);
};

#endif
