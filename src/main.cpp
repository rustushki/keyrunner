#include "Level.h"
#include "KeyRunner.h"

Level level;
State state;

int main(int argc, char** argv) {

	// Simple Support for Starting on a Different Level.
	int levelNum = 1;
	if (argc > 1) {
		levelNum = atoi(argv[1]);
	}

	KeyRunner::play(levelNum);

	return 0;
}
