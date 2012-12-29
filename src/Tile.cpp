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
 * isConveyor - Return true if the Tile is a conveyor tile.
 */
bool Tile::isConveyor() const {
	return (    this->getType() == TILETYPE_CONVEY_UP
	         || this->getType() == TILETYPE_CONVEY_DOWN
	         || this->getType() == TILETYPE_CONVEY_RIGHT
	         || this->getType() == TILETYPE_CONVEY_LEFT);
}


Tile* Tile::up() const{
	int x = this->x + 0;
	int y = this->y - 1;
	if (y < 0) {
		y = GRID_HEIGHT-1;
	}
	return this->level->getTile(x, y);
}

Tile* Tile::down() const{
	int x = this->x + 0;
	int y = this->y + 1;
	if (y >= GRID_HEIGHT) {
		y = 0;
	}
	return this->level->getTile(x, y);
}

Tile* Tile::left() const{
	int x = this->x - 1;
	int y = this->y + 0;
	if (x < 0) {
		x = GRID_WIDTH-1;
	}
	return this->level->getTile(x, y);
}

Tile* Tile::right() const{
	int x = this->x + 1;
	int y = this->y + 0;
	if (x >= GRID_WIDTH) {
		x = 0;
	}
	return this->level->getTile(x, y);
}

/* ------------------------------------------------------------------------------
 * getConveyorDirection - Return the direction the conveyor is pointing
 * towards.
 */
Direction Tile::getConveyorDirection() const {
	if (this->getType() == TILETYPE_CONVEY_UP) {
		return DIRECTION_UP;

	} else if (this->getType() == TILETYPE_CONVEY_DOWN) {
		return DIRECTION_DOWN;

	} else if (this->getType() == TILETYPE_CONVEY_RIGHT) {
		return DIRECTION_RIGHT;

	} else if (this->getType() == TILETYPE_CONVEY_LEFT) {
		return DIRECTION_LEFT;

	}

	std::cout << "Non-conveyor tile queried for direction." << std::endl;
	exitGame();

	// Should never execute.
	return DIRECTION_UP;
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

/* ------------------------------------------------------------------------------
 * getNextConveyorTile - Return the 'drop off' spot of this tile if its a
 * conveyor tile.  The strategy here is relatively simple:
 *    Check each tile clock wise inclusive from the current tile:
 *        1. If the tile is a conveyor, return that tile.
 * 
 *        2. If the tile is not wall or conveyor, make that tile the backup in
 *        case no conveyor is found. (second place)
 * 
 *        3. If there is no suitable second place, return the current tile in
 *        the conveyor belt sequence.
 */
Tile* Tile::getNextConveyorTile() const {
	if (!this->isConveyor()) {
		std::cout << "Trying to get next conveyor tile from non conveyor tile." << std::endl;
		exitGame();
	}

	Direction dir = this->getConveyorDirection();
	Direction origDir = dir;

	Direction oppDir;
	if (origDir == DIRECTION_UP) {
		oppDir = DIRECTION_DOWN;
	} else if (origDir == DIRECTION_DOWN) {
		oppDir = DIRECTION_UP;
	} else if (origDir == DIRECTION_RIGHT) {
		oppDir = DIRECTION_LEFT;
	} else if (origDir == DIRECTION_LEFT) {
		oppDir = DIRECTION_RIGHT;
	}

	Tile* tryTile = NULL;
	Tile* secondPlace = NULL;
	Tile* thirdPlace = (Tile*)this;


	while (true) {

		tryTile = this->getTileInDirection(dir);

		if (!tryTile->isWall() && !tryTile->isConveyor() && secondPlace == NULL) {
			secondPlace = tryTile;

		// Prefer adjacent conveyor belt tiles.  Explicitly do not place the
		// player on a conveyor belt tile in the exact opposite direction of
		// current travel.
		} else if (tryTile->isConveyor()) {

			if (dir != oppDir) {
				Direction dir = tryTile->getConveyorDirection();

				Tile* check = tryTile->getTileInDirection(dir);

				if (check != this) {
					return tryTile;
				}
			}
		}

		if (dir == DIRECTION_UP) {
			dir = DIRECTION_RIGHT;
		} else if (dir == DIRECTION_RIGHT) {
			dir = DIRECTION_DOWN;
		} else if (dir == DIRECTION_DOWN) {
			dir = DIRECTION_LEFT;
		} else if (dir == DIRECTION_LEFT) {
			dir = DIRECTION_UP;
		}

		if (dir == DIRECTION_COUNT) {
			dir = DIRECTION_UP;
		}

		if (dir == origDir) {
			break;
		}

	}

	if (secondPlace != NULL) {
		return secondPlace;
	}

	return thirdPlace;


}

/* ------------------------------------------------------------------------------
 * getTileInDirection - Given a direction, return the tile to that direction
 * from this tile..
 */
Tile* Tile::getTileInDirection(Direction dir) const {
	if (dir == DIRECTION_UP) {
		return this->up();
	} else if (dir == DIRECTION_DOWN) {
		return this->down();
	} else if (dir == DIRECTION_RIGHT) {
		return this->right();
	} else if (dir == DIRECTION_LEFT) {
		return this->left();
	}

	return NULL;
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

/* ------------------------------------------------------------------------------
 * ClearChangedTiles - Remove all tiles from the changed tile array.
 */
void Tile::ClearChangedTiles() {
	Tile::ChangedTiles.clear();
}

void Tile::AddChangedTile(Tile* tile) {
	Tile::ChangedTiles.push_back(tile);
}
