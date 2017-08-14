#ifndef EDIT_CONTROLLER_HPP
#define EDIT_CONTROLLER_HPP

#include "../controller/BoardController.hpp"
#include "../uitk/ButtonView.hpp"
#include "../model/EditorBoardModel.hpp"

class EditController : public BoardController {
public:
    EditController(EditorBoardModel* model, Display* display, uint8_t editingLevel);
    ~EditController() override = default;

private:
    void updateModel(long frameDuration) override;
    EditorBoardModel* getModel() const override;
    bool checkExitConditions() const override;

    View* createRectangle() const;
    View* createBoard() const;
    View* createSaveButton(View* board, uint8_t buttonSpacing) const;
    View* createExitButton(View* board, int buttonSpacing) const;
    View* createTileTypeButton(View* board, TileType tileType, uint8_t buttonSpacing) const;
};

#endif
