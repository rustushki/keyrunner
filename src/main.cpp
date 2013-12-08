#include "KeyRunner.hpp"
#include "Options.hpp"

int main(int argc, char** argv) {

    Options::parse(argc, argv);

    State startState = Options::getInitialState();
    if (startState == PLAY) {
        KeyRunner::play();

    } else if (startState == EDIT) {
        // Do something else.
    }

    return 0;
}
