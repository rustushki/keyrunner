#ifndef CONVEYOR_HPP
#define CONVEYOR_HPP

#include <stdint.h>
#include <vector>

class Tile;

class ConveyorAnimation {
public:
	ConveyorAnimation(std::vector<Tile*>);
	~ConveyorAnimation();

	static bool TileInConveyor(Tile* tile);
	static bool StartConveyors();
	static void ClearConveyors();

	bool startAnimation();

private:
	
	std::vector<Tile*> conveyorTiles;
	bool hasTile(Tile*) const;

	uint16_t nextTileToStart;

	static std::vector<ConveyorAnimation*> Conveyors;
};

#endif//CONVEYOR_HPP
