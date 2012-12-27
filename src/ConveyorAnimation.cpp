#include <iostream>
#include "ConveyorAnimation.h"
#include "KeyRunner.h"
#include "Tile.h"

std::vector<ConveyorAnimation*> ConveyorAnimation::Conveyors;

ConveyorAnimation::ConveyorAnimation(std::vector<Tile*> conveyorTiles) {
	this->conveyorTiles = conveyorTiles;

	ConveyorAnimation::Conveyors.push_back(this);

	this->nextTileToStart = 0;
}

ConveyorAnimation::~ConveyorAnimation(){ }

/* ------------------------------------------------------------------------------
 * startAnimation - Attempts to start the next tile in the chained conveyor
 * belt animation.  Returns true if all tiles have been started.  For best
 * results, call this method each time a new screen frame is blitted.
 */
bool ConveyorAnimation::startAnimation() {
	if (this->nextTileToStart >= this->conveyorTiles.size()) {
		return true;
	}

	Tile* currentTile = NULL;
	if (this->nextTileToStart == 0) {

		// Always start the first tile in the belt.
		currentTile = this->conveyorTiles[0];
		currentTile->getAnimation()->play();
		Tile::PushAnimatedTile(currentTile);
		this->nextTileToStart++;

	} else {
		currentTile = this->conveyorTiles[this->nextTileToStart-1];

		if ((currentTile->getAnimation()->getCurrentStill()+1) % 8 == 0) {

			Tile* tile = this->conveyorTiles[this->nextTileToStart];
			this->conveyorTiles[this->nextTileToStart]->getAnimation()->play();
			Tile::PushAnimatedTile(tile);

			this->nextTileToStart++;
		}
	}

	return (this->nextTileToStart >= this->conveyorTiles.size());

}

bool ConveyorAnimation::hasTile(Tile* tile) const {
	for (int x = 0; x < this->conveyorTiles.size(); x++) {
		if (tile == this->conveyorTiles[x]) {
			return true;
		}
	}

	return false;
}

/* ------------------------------------------------------------------------------
 * TileInConveyor - Check all conveyors to see if the provided tile is already
 * in a conveyor belt.
 */
bool ConveyorAnimation::TileInConveyor(Tile* tile) {

	for (int c = 0; c < ConveyorAnimation::Conveyors.size(); c++) {
		ConveyorAnimation* conveyor = ConveyorAnimation::Conveyors[c];
		if (conveyor->hasTile(tile)) {
			
			return true;

		}
	}

	return false;
}

/* ------------------------------------------------------------------------------
 * StartConveyors - For each ConveyorAnimation currently loaded, attempt to
 * start the next tile in each.  This method ought to be called once per frame
 * blit until it returns true.
 */
bool ConveyorAnimation::StartConveyors() {
	bool allStarted = true;
	for (int x = 0; x < ConveyorAnimation::Conveyors.size(); x++) {
		ConveyorAnimation* ca = ConveyorAnimation::Conveyors[x];
		bool started = ca->startAnimation();

		allStarted &= started;
	}

	return allStarted;
}