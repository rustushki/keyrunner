#include <iostream>
#include "Animation.h"
#include "AnimationType.h"
#include "KeyRunner.h"
#include "TileType.h"
#include "Tile.h"

Tile::Tile(TileType type) {
	AnimationType at = Tile::TileTypeToAnimType(type);
	this->anim = Animation::AnimationFactory(at);
	this->type = type;
}

Tile::~Tile() {
	delete this->anim;
}

Animation* Tile::getAnimation() const {
	return anim;
}

AnimationType Tile::TileTypeToAnimType(TileType tt) {
	switch (tt) {
		case TILETYPE_EMPTY:
			return ANIMATION_TYPE_EMPTY;

		case TILETYPE_WALL:
			return ANIMATION_TYPE_WALL;

		case TILETYPE_DOOR:
			return ANIMATION_TYPE_DOOR;

		case TILETYPE_TELEPORTER_RED:
			return ANIMATION_TYPE_TELEPORTER_RED;

		case TILETYPE_TELEPORTER_GREEN:
			return ANIMATION_TYPE_TELEPORTER_GREEN;

		case TILETYPE_TELEPORTER_BLUE:
			return ANIMATION_TYPE_TELEPORTER_BLUE;

		case TILETYPE_CONVEY_UP:
			return ANIMATION_TYPE_CONVEY_UP;

		case TILETYPE_CONVEY_DOWN:
			return ANIMATION_TYPE_CONVEY_DOWN;

		case TILETYPE_CONVEY_LEFT:
			return ANIMATION_TYPE_CONVEY_LEFT;

		case TILETYPE_CONVEY_RIGHT:
			return ANIMATION_TYPE_CONVEY_RIGHT;

		default:
			std::cout << "Could not determine AnimationType." << std::endl;
			std::cout << "Invalid TileType." << std::endl;
			exitGame();
			break;
	}

	// Should not arrive here.
	std::cout << "Logic error detected." << std::endl;
	exitGame();
	
}

TileType Tile::getType() const {
	return this->type;
}
