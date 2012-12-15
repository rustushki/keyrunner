#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "AnimationType.h"
#include "SpriteSheet.h"

typedef unsigned int uint;

class Animation {

public:
	static Animation* AnimationFactory(AnimationType at);
	void advance(uint x, uint y);

private:
	Animation(AnimationType at);
	static uint              GetSPS      (AnimationType at); 
	static std::string       GetSpriteSFN(AnimationType at); 
	static std::vector<uint> GetFrameSize(AnimationType at); 
	static std::vector<uint> GetFrameList(AnimationType at);
	SpriteSheet* sheet;
	uint sps;
	bool firstBlit;
	std::vector<uint> frameList;
	uint currentFrame;
	AnimationType type;

};

#endif//ANIMATION_H
