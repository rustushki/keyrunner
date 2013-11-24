#include "Level.h"
#include "KeyRunner.h"

Level level;
State state;

int main(int argc, char** argv) {

	// Simple Support for Starting on a Different Level.
	if (argc > 1) {
		levelNum = atoi(argv[1]);
	} else {
		levelNum  = 1;
	}

	KeyRunner kr;
	kr.play(levelNum);

	return 0;
}
