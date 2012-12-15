#ifndef TILE_H
#define TILE_H

#include "TileType.h"
#include "AnimationType.h"
class Animation;

class Tile {

public:
	Tile(TileType type);
	~Tile();

	Animation* getAnimation() const;
	static AnimationType TileTypeToAnimType(TileType tt);

	TileType getType() const;

private:
	TileType type;
	AnimationType animType;
	Animation* anim;
};

#endif//TILE_H
