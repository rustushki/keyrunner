#ifndef EDIT_CONTROLLER_HPP
#define EDIT_CONTROLLER_HPP

#include "../controller/BoardController.hpp"
#include "../uitk/ButtonView.hpp"
#include "../model/EditorBoardModel.hpp"

class EditorController : public BoardController {
public:
    EditorController(EditorBoardModel* model, Display* display, uint8_t editingLevel);
    ~EditorController() override = default;

private:
    void updateModel(long frameDuration) override;
    EditorBoardModel* getModel() const override;
    bool isStillExecuting() const override;

    View* createRectangle() const;
    View* createBoard() const;
    View* createSaveButton(View* board, uint8_t buttonSpacing) const;
    View* createExitButton(View* board, int buttonSpacing) const;
    View* createTileTypeButton(View* board, TileType tileType, uint8_t buttonSpacing) const;
};

#endif
