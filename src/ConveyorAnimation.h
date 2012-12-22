#ifndef CONVEYOR_H
#define CONVEYOR_H

#include <vector>
class Tile;

class ConveyorAnimation {
public:
	ConveyorAnimation(std::vector<Tile*>);
	~ConveyorAnimation();

	static bool TileInConveyor(Tile* tile);
private:
	
	std::vector<Tile*> conveyorTiles;
	bool hasTile(Tile*) const;

	static std::vector<ConveyorAnimation*> Conveyors;
};

#endif//CONVEYOR
