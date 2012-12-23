#ifndef CONVEYOR_H
#define CONVEYOR_H

#include <vector>
class Tile;

typedef unsigned int uint;

class ConveyorAnimation {
public:
	ConveyorAnimation(std::vector<Tile*>);
	~ConveyorAnimation();

	static bool TileInConveyor(Tile* tile);
	static bool StartConveyors();

	bool startAnimation();

private:
	
	std::vector<Tile*> conveyorTiles;
	bool hasTile(Tile*) const;

	uint nextTileToStart;

	static std::vector<ConveyorAnimation*> Conveyors;
};

#endif//CONVEYOR
