#include <iostream>
#include "Animation.h"
#include "AnimationType.h"
#include "KeyRunner.h"
#include "Level.h"
#include "TileType.h"
#include "Tile.h"

std::vector<Tile*> Tile::AnimatedTiles;
std::vector<Tile*> Tile::ChangedTiles;


Tile::Tile(TileType type, uint x, uint y, Level* level) {
	AnimationType at = Tile::TileTypeToAnimType(type);
	this->anim = Animation::AnimationFactory(at);
	
	if (this->anim->isAnimating()) {
		Tile::PushAnimatedTile(this);
	}

	this->type = type;
	this->x = x;
	this->y = y;
	this->level = level;
}

Tile::~Tile() {
	delete this->anim;
}

/* ------------------------------------------------------------------------------
 * PushAnimatedTile - Push the provided animated tile onto the vector of tiles
 * which have animations which require advancing.
 */
void Tile::PushAnimatedTile(Tile* tile) {
	Tile::AnimatedTiles.push_back(tile);
}

/* ------------------------------------------------------------------------------
 * ClearAnimatedTiles - Empty the vector containing Tiles which have Animations
 * which would otherwise be rendered on screen updates.
 */
void Tile::ClearAnimatedTiles() {
	Tile::AnimatedTiles.clear();
}

/* ------------------------------------------------------------------------------
 * AdvanceAnimatables - Iterate the list of Animations which require advancing,
 * and advance each one.
 */
void Tile::AnimateTiles() {
	for (uint x = 0; x < Tile::AnimatedTiles.size(); x++) {
		Tile* tile = Tile::AnimatedTiles[x];
		if (tile->getAnimation()->advance()) {
			Tile::AddChangedTile(tile);
		}
	}
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

void Tile::draw() {
	
	const uint tileSize = 25;

	// Determine the coordinate to draw the tile animation..
	uint xp = this->x*tileSize;
	uint yp = this->y*tileSize;

	this->getAnimation()->move(xp, yp);
	this->getAnimation()->blit();

	// Redraw the Key.
	if (this->hasKey()) {
		KeyAnim->move(xp, yp);
		KeyAnim->blit();
	}
	
	// Redraw the Player.
	if (this->hasPlayer()) {
		PlayerAnim->move(xp, yp);
		PlayerAnim->blit();
	}

}

void Tile::RedrawChangedTiles() {

	while (!Tile::ChangedTiles.empty()) {

		// Get pair to update.
		Tile* t = Tile::ChangedTiles.back();
		
		// Redraw the tile referenced by that pair.
		t->draw();

		// Remove that pair from the changed tiles list.
		Tile::ChangedTiles.pop_back();
	}

}

void Tile::AddChangedTile(Tile* tile) {
	Tile::ChangedTiles.push_back(tile);
	std::cout << Tile::ChangedTiles.size() << std::endl;
}
