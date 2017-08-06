#include "../view/AnimationFactory.hpp"
#include "GameManager.hpp"

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

    GameManager gameManager{options};
    gameManager.loop();
}

