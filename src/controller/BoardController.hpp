#ifndef BOARD_CONTROLLER_HPP
#define BOARD_CONTROLLER_HPP

#include "DisplayController.hpp"
#include "../controller/Controller.hpp"
#include "../model/LevelManager.hpp"

class BoardController : public DisplayController {
public:
    BoardController(BoardModel *model, Display *display);

protected:
    LevelManager* getLevelManager() const;

private:
    LevelManager* levelManager;
};

#endif
