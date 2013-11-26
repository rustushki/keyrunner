#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "Level.h"

class LevelLoader {

public:
	static Level* Load(int level);
	static uint16_t GetTotal();

private:
	static bool ParseLine(Level* level, int& parseX, int& parseY, std::string line);
	static void ParseVar(Level* level, std::string line);
};

#endif//LEVELLOADER_H
