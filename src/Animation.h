#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "AnimationType.h"
#include "SpriteSheet.h"

typedef unsigned int uint;

class Animation {

public:
	Animation(SpriteSheet* sheet, uint sps, AnimationType at);
	void advance();

private:
	std::vector<uint> GetFrameList(AnimationType at);
	SpriteSheet* sheet;
	uint sps;
	bool firstBlit;
	std::vector<uint> frameList;
	uint currentFrame;

};

#endif//ANIMATION_H
