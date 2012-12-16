#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "AnimationType.h"
#include "SpriteSheet.h"

typedef unsigned int uint;

class Animation {

public:
	static Animation* AnimationFactory(AnimationType at);
	static void PushAnimatable(Animation* anim);
	static void AdvanceAnimatables();
	static void ClearAnimatables();
	void advance();
	void blit();
	void move(uint x, uint y);

private:
	Animation(AnimationType at);
	static uint              GetSPS      (AnimationType at); 
	static std::string       GetSpriteSFN(AnimationType at); 
	static std::vector<uint> GetFrameSize(AnimationType at); 
	static std::vector<uint> GetFrameList(AnimationType at);
	static std::vector<Animation*> Animatables;
	SpriteSheet* sheet;
	uint sps;
	bool firstBlit;
	std::vector<uint> frameList;
	uint currentFrame;
	AnimationType type;
	uint x;
	uint y;

	uint framesPerStill;
	uint advanceCount;

};

#endif//ANIMATION_H
