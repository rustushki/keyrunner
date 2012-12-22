#include <iostream>
#include "ConveyorAnimation.h"
#include "KeyRunner.h"
#include "Tile.h"

std::vector<ConveyorAnimation*> ConveyorAnimation::Conveyors;

ConveyorAnimation::ConveyorAnimation(std::vector<Tile*> conveyorTiles) {
	this->conveyorTiles = conveyorTiles;

	ConveyorAnimation::Conveyors.push_back(this);
}

ConveyorAnimation::~ConveyorAnimation(){ }

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
