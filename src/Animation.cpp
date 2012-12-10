#include <iostream>
#include "Animation.h"
#include "KeyRunner.h"

#define FL(x,y) frameList.push_back(x);frameList.push_back(y);

Animation::Animation(SpriteSheet* sheet, uint sps, AnimationType at) {
	this->sheet = sheet;
	this->sps = sps;
	this->firstBlit = true;
	this->currentFrame = 0;
	
	this->frameList = Animation::GetFrameList(at);
}

void Animation::advance() {
	
	if (this->firstBlit) {

		uint x = this->currentFrame*2 + 0;
		uint y = this->currentFrame*2 + 1;

		SDL_Rect r;
		r.x = 25;
		r.y = 25;
		r.w = 25;
		r.h = 25;

		this->sheet->blitFrame(x, y, r); 

		this->currentFrame++;
	}

}

/* ------------------------------------------------------------------------------
 * GetFrameList - Given an AnimationType, return a vector of unsigned ints
 * which denote the list of frames within a SpriteSheet for that kind of
 * animation.
 */
std::vector<uint> Animation::GetFrameList(AnimationType at) {
	std::vector<uint> frameList;

	switch (at) {

		case ANIMATION_TYPE_CONVEY_DOWN:
			FL(1,0)FL(1,1)FL(1,2)FL(1,3)
			FL(1,4)FL(1,5)FL(1,6)FL(1,7)
			FL(1,8)FL(1,9)FL(1,10)FL(1,11)
			break;

		case ANIMATION_TYPE_CONVEY_UP:
			FL(2,0)FL(2,1)FL(2,2)FL(2,3)
			FL(2,4)FL(2,5)FL(2,6)FL(2,7)
			FL(2,8)FL(2,9)FL(2,10)FL(2,11)
			break;

		case ANIMATION_TYPE_CONVEY_LEFT:
			FL(0,0)FL(0,1)FL(0,2)FL(0,3)
			FL(0,4)FL(0,5)FL(0,6)FL(0,7)
			FL(0,8)FL(0,9)FL(0,10)FL(0,11)
			break;

		case ANIMATION_TYPE_CONVEY_RIGHT:
			FL(3,0)FL(3,1)FL(3,2)FL(3,3)
			FL(3,4)FL(3,5)FL(3,6)FL(3,7)
			FL(3,8)FL(3,9)FL(3,10)FL(3,11)
			break;

		default:
			std::cout << "Invalid Animation Type." << std::endl;
			exitGame();
			break;

	}

	return frameList;
}
