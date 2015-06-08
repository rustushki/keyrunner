#ifndef STATE_HPP
#define STATE_HPP

// Game State.
enum State {
      PLAY     // User is playing.
    , QUIT     // User has indicated he wants to quit.
    , START    // Build the next level.
    , LOSE     // User has lost.  Display lose animation.
    , RESTART  // Start over at level 1, reset score, etc..
    , WIN      // User has won the game.
    , EDIT     // User wants to edit levels
};

#endif//STATE_HPP
