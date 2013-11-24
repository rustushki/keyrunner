#ifndef STATE_H
#define STATE_H

// Game State.
enum State {
	  PLAY     // User is playing.
	, QUIT     // User has indicated he wants to quit.
	, START    // Build the next level.
	, LOSE     // User has lost.  Display lose animation.
	, RESTART  // Start over at level 1, reset score, etc..
	, WIN      // User has won the game.
};

#endif//STATE_H
