#ifndef EDIT_CONTROLLER_HPP
#define EDIT_CONTROLLER_HPP

#include "../controller/BaseController.hpp"
#include "../controller/Options.hpp"
#include "../uitk/ButtonView.hpp"
#include "../model/EditorBoardModel.hpp"

class EditController : public BaseController {
public:
    EditController(EditorBoardModel* model, Display* display, Options* options);
    virtual ~EditController() override;

private:
    virtual void updateModel(long frameDuration) override;
    virtual void processInput() override;
    EditorBoardModel* getModel() const override;
    virtual bool checkExitConditions() const override;

    View* createRectangle() const;
    View* createBoard() const;
    View* createSaveButton(View* board, uint8_t buttonSpacing) const;
    View* createExitButton(View* board, int buttonSpacing) const;
    View* createTileTypeButton(View* board, TileType tileType, uint8_t buttonSpacing) const;
};

#endif
