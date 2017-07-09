#ifndef STATE_HPP
#define STATE_HPP

// Game State.
enum State {
      PLAY     // User is playing.
    , QUIT     // User has indicated he wants to quit.
    , LOSE     // User has lost.  Display lose animation.
    , WIN      // User has won the game.
    , EDIT     // User wants to edit levels
};

#endif
