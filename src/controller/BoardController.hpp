#ifndef BOARD_CONTROLLER_HPP
#define BOARD_CONTROLLER_HPP

#include "../controller/Controller.hpp"
#include "../controller/DisplayController.hpp"
#include "../model/BinaryLevelManager.hpp"

class BoardController : public DisplayController {
public:
    BoardController(BoardModel* model, Display* display);

protected:
    LevelManager* getLevelManager() const;

private:
    LevelManager* levelManager;
};

#endif
