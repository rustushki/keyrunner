#include <iostream>
#include "Animation.h"
#include "AnimationType.h"
#include "KeyRunner.h"
#include "Level.h"
#include "TileType.h"
#include "Tile.h"

Tile::Tile(TileType type, uint x, uint y, Level* level) {
	AnimationType at = Tile::TileTypeToAnimType(type);
	this->anim = Animation::AnimationFactory(at);
	this->type = type;
	this->x = x;
	this->y = y;
	this->level = level;
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

/* ------------------------------------------------------------------------------
 * isTeleporter - Return true if the tile is a teleporter tile.
 */
bool Tile::isTeleporter() const {
	TileType tt = this->getType();

	return (    tt == TILETYPE_TELEPORTER_RED
	         || tt == TILETYPE_TELEPORTER_GREEN
	         || tt == TILETYPE_TELEPORTER_BLUE);
}

bool Tile::isDoor() const {
	return (this->getType() == TILETYPE_DOOR);
}

bool Tile::isWall() const {
	return (this->getType() == TILETYPE_WALL);
}

/* ------------------------------------------------------------------------------
 * hasPlayer - Determine whether this tile has the player.
 */
bool Tile::hasPlayer() const {
	uint x = this->getX();
	uint y = this->getY();
	return (this->level->hasPlayer(x, y));
}

/* ------------------------------------------------------------------------------
 * hasKey - Determine whether this tile has the key.
 */
bool Tile::hasKey() const {
	uint x = this->getX();
	uint y = this->getY();
	return (this->level->hasKey(x, y));
}

uint Tile::getX() const {
	return this->x;
}

uint Tile::getY() const {
	return this->y;
}
