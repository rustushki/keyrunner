#include "../controller/GameController.hpp"

// Objects that don't have a home yet
AnimationFactory* animationFactory;


/**
 * Entry point into KeyRunner.
 * @param argc count of arguments
 * @param argv array of string arguments
 * @return exit code
 */
int main(int argc, char** argv) {
    // Parse the command line options
    Options options;
    options.parse(argc, argv);

    // Create a GameModel and GameController and start the game
    auto gameModel = new GameModel();
    GameController gameController{gameModel, options};
    gameController.gameLoop();
    delete gameModel;
}

