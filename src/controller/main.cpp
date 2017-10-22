#include "../controller/GameController.hpp"

/**
 * Entry point into KeyRunner.
 * @param argc count of arguments
 * @param argv array of string arguments
 * @return exit code
 */
int main(int argc, char** argv) {
    // Create a GameModel and GameController and start the game
    auto gameModel = new GameModel();
    GameController gameController{gameModel, argc, argv};
    gameController.execute();
    delete gameModel;
}
