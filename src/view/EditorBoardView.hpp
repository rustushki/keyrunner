#ifndef EDITOR_BOARD_VIEW_HPP
#define EDITOR_BOARD_VIEW_HPP

#include "../view/BoardView.hpp"
#include "../model/EditorBoardModel.hpp"

class EditorBoardView : public BoardView {
public:
    EditorBoardView(EditorBoardModel* model, const SDL_Rect &rect, AnimationFactory* animationFactory);
    void draw(SDL_Renderer* renderer) override;
    EditorBoardModel* getModel() const override;

private:
    void drawCursorTile(SDL_Renderer* renderer, bool justHighlight);
};

#endif
