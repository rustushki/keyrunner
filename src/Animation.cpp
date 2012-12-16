#include <iostream>
#include <sstream>
#include "Animation.h"
#include "KeyRunner.h"

#define FL(x,y) frameList.push_back(x);frameList.push_back(y);

std::vector<Animation*> Animation::Animatables;


/* ------------------------------------------------------------------------------
 * Animation - Build an animation based upon it's type.
 */
Animation::Animation(AnimationType at) {

	// Get Full Path to the Filename of the SpriteSheet associated with this
	// Animation.
	std::string filename = Animation::GetSpriteSFN(at);

	// Determine the height and width of this animation.
	std::vector<uint> frameSize = Animation::GetFrameSize(at);
	uint width  = frameSize[0];
	uint height = frameSize[1];

	// Build the SpriteSheet.
	SpriteSheet* ss = new SpriteSheet(filename, width, height);
	this->sheet = ss;

	// Set the stills per second.
	this->sps = Animation::GetSPS(at);

	// Get List of Frames for this kind of Animation.
	this->frameList = Animation::GetFrameList(at);

	this->firstBlit    = true;
	this->currentFrame = 0;
	this->type         = at;

	int fps = 25;

	if (this->sps != 0) {
		this->framesPerStill = fps/this->sps;
		Animation::PushAnimatable(this);
	}

	this->advanceCount = 0;
}

/* ------------------------------------------------------------------------------
 * AnimationFactory - Return an Animation given an AnimationType.
 */
Animation* Animation::AnimationFactory(AnimationType at) {

	return new Animation(at);

}

/* ------------------------------------------------------------------------------
 * PushAnimatable - Push the provided animation onto the vector of animations
 * which require advancing.
 */
void Animation::PushAnimatable(Animation* anim) {
	Animation::Animatables.push_back(anim);
}

/* ------------------------------------------------------------------------------
 * ClearAnimatables - Empty the vector containing the Animations to be rendered
 * on screen updates.
 */
void Animation::ClearAnimatables() {
	Animation::Animatables.clear();
}

/* ------------------------------------------------------------------------------
 * AdvanceAnimatables - Iterate the list of Animations which require advancing,
 * and advance each one.
 */
void Animation::AdvanceAnimatables() {
	for (uint x = 0; x < Animation::Animatables.size(); x++) {
		Animation::Animatables[x]->advance();
	}
}

/* ------------------------------------------------------------------------------
 * advance - Increment the advanceCount.  If the advanceCount equals the number
 * of frame required per still, the increment the current frame and blit it in
 * place.
 *
 * Circular wrap the still list.
 */
void Animation::advance() {

	this->advanceCount++;
	
	if (this->advanceCount == framesPerStill) {

		this->advanceCount = 0;
		this->blit();
		this->currentFrame++;

		if (this->currentFrame >= this->frameList.size()/2) {
			this->currentFrame = 0;
		}
	}

}

/* ------------------------------------------------------------------------------
 * blit - Draw the current frame of the animation onto the screen.
 */
void Animation::blit() {

	// Look into the frame list to determine the logical coordinates of the
	// frame in the spritesheet.
	uint frameXc = this->currentFrame*2 + 0;
	uint frameYc = this->currentFrame*2 + 1;
	uint frameX  = this->frameList[frameXc];
	uint frameY  = this->frameList[frameYc];

	SDL_Rect r;
	r.x = this->x;
	r.y = this->y;
	r.w = this->sheet->getWidth();
	r.h = this->sheet->getHeight();

	this->sheet->blitFrame(frameX, frameY, r); 


}

/* ------------------------------------------------------------------------------
 * move - Move the blitting location of this Animation elsewhere.
 */
void Animation::move(uint x, uint y) {
	this->x = x;
	this->y = y;
}

/* ------------------------------------------------------------------------------
 * GetSPS - Given an animation type, return the stills per second count.  This
 * may go away in the future as objects are introduced for on screen elements.
 * Such objects might need to adjust and regulate sps.
 */
uint Animation::GetSPS(AnimationType at) {
	uint sps = 0;

	switch (at) {

		case ANIMATION_TYPE_EMPTY:
		case ANIMATION_TYPE_WALL:
		case ANIMATION_TYPE_DOOR:
		case ANIMATION_TYPE_TELEPORTER_RED:
		case ANIMATION_TYPE_TELEPORTER_GREEN:
		case ANIMATION_TYPE_TELEPORTER_BLUE:
		case ANIMATION_TYPE_KEY:
		case ANIMATION_TYPE_PUMPKIN:
			sps = 0;
			break;

		case ANIMATION_TYPE_CONVEY_DOWN:
		case ANIMATION_TYPE_CONVEY_UP:
		case ANIMATION_TYPE_CONVEY_LEFT:
		case ANIMATION_TYPE_CONVEY_RIGHT:
			sps = 7;
			break;

		default:
			std::cout << "Could not determine sps." << std::endl;
			std::cout << "Invalid Animation Type." << std::endl;
			exitGame();
			break;

	}

	return sps;
}

/* ------------------------------------------------------------------------------
 * GetSpriteSFN - Given an animation type, determine the path to the
 * spritesheet associated with that animation.
 */
std::string Animation::GetSpriteSFN(AnimationType at) {

	std::string fn;

	switch (at) {

		case ANIMATION_TYPE_EMPTY:
			fn = "tile.png";
			break;

		case ANIMATION_TYPE_WALL:
			fn = "wall.png";
			break;

		case ANIMATION_TYPE_DOOR:
			fn = "door.png";
			break;

		case ANIMATION_TYPE_TELEPORTER_RED:
			fn = "teleporter_red.png";
			break;

		case ANIMATION_TYPE_TELEPORTER_GREEN:
			fn = "teleporter_green.png";
			break;

		case ANIMATION_TYPE_TELEPORTER_BLUE:
			fn = "teleporter_blue.png";
			break;

		case ANIMATION_TYPE_CONVEY_DOWN:
		case ANIMATION_TYPE_CONVEY_UP:
		case ANIMATION_TYPE_CONVEY_LEFT:
		case ANIMATION_TYPE_CONVEY_RIGHT:
			fn = "conveyor.png";
			break;

		case ANIMATION_TYPE_KEY:
			fn = "key.png";
			break;

		case ANIMATION_TYPE_PUMPKIN:
			fn = "pumpkin.png";
			break;

		default:
			std::cout << "Could not determine sprite sheet." << std::endl;
			std::cout << "Invalid Animation Type:" << at << std::endl;
			exitGame();
			break;

	}

	
	// Prepend the image path.
	std::stringstream ss;
	ss << IMGPATH << fn;

	// Return path to the spritesheet file.
	return ss.str();
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

		case ANIMATION_TYPE_EMPTY:
		case ANIMATION_TYPE_WALL:
		case ANIMATION_TYPE_DOOR:
		case ANIMATION_TYPE_TELEPORTER_RED:
		case ANIMATION_TYPE_TELEPORTER_GREEN:
		case ANIMATION_TYPE_TELEPORTER_BLUE:
		case ANIMATION_TYPE_KEY:
		case ANIMATION_TYPE_PUMPKIN:
			FL(0,0);
			break;

		default:
			std::cout << "Could not determine frame list." << std::endl;
			std::cout << "Invalid Animation Type." << std::endl;
			exitGame();
			break;

	}

	return frameList;
}

/* ------------------------------------------------------------------------------
 * GetFrameSize - Given an AnimationType, return the frame size of the
 * SpriteSheet associated with that Animation.
 */
std::vector<uint> Animation::GetFrameSize(AnimationType at) {
	std::vector<uint> frameSize;

	switch (at) {

		case ANIMATION_TYPE_EMPTY:
		case ANIMATION_TYPE_WALL:
		case ANIMATION_TYPE_DOOR:
		case ANIMATION_TYPE_TELEPORTER_RED:
		case ANIMATION_TYPE_TELEPORTER_GREEN:
		case ANIMATION_TYPE_TELEPORTER_BLUE:
		case ANIMATION_TYPE_CONVEY_UP:
		case ANIMATION_TYPE_CONVEY_DOWN:
		case ANIMATION_TYPE_CONVEY_LEFT:
		case ANIMATION_TYPE_CONVEY_RIGHT:
		case ANIMATION_TYPE_KEY:
		case ANIMATION_TYPE_PUMPKIN:
			frameSize.push_back(25);
			frameSize.push_back(25);
			break;
		
		default:
			std::cout << "Could not determine frame size." << std::endl;
			std::cout << "Invalid Animation Type." << std::endl;
			exitGame();
			break;

	}

	return frameSize;
}
