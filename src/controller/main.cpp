#include "../controller/Controller.hpp"
#include "../controller/Options.hpp"

/**
 * Entry point into KeyRunner.
 * @param argc count of arguments
 * @param argv array of string arguments
 * @return exit code
 */
int main(int argc, char** argv) {

    Options::parse(argc, argv);

    // Based on the initial state of the game, choose the appropriate thread
    // managing function.
    //
    // Note: The thread managing functions manage a different set of thread
    // functions--depending on the initial state of the program.
    State startState = Options::getInitialState();

    Controller controller;

    // Play Mode
    if (startState == PLAY) {
        controller.play();

    // Editor Mode
    } else if (startState == EDIT) {
        controller.edit();
    }

    return 0;
}
