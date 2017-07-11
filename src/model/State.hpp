#ifndef STATE_HPP
#define STATE_HPP

// Game State.
enum State {
    TITLE, // User sees the title screen
    PLAY,  // User is playing
    QUIT,  // User wants to quit
    LOSE,  // User has lost
    WIN,   // User has won the game
    EDIT,  // User wants to edit levels
};

#endif
