#include "KeyRunner.hpp"
#include "Options.hpp"
#include "../model/PlayModel.hpp"

KeyRunner keyRunner;

int main(int argc, char** argv) {

    Options::parse(argc, argv);

    // Based on the initial state of the game, choose the appropriate thread
    // managing function.
    //
    // Note: The thread managing functions manage a different set of thread
    // functions--depending on the initial state of the program.
    State startState = Options::getInitialState();

    // Normal Gameplay Mode
    if (startState == PLAY) {
        keyRunner.play();

    // Editor Mode
    } else if (startState == EDIT) {
        keyRunner.edit();
    }

    return 0;
}
