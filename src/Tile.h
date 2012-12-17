#ifndef TILE_H
#define TILE_H

#include "TileType.h"
#include "AnimationType.h"
class Animation;

class Tile {

public:
	Tile(TileType type, uint x, uint y);
	~Tile();

	Animation* getAnimation() const;
	static AnimationType TileTypeToAnimType(TileType tt);

	TileType getType() const;

	uint getX() const;
	uint getY() const;

	bool isTeleporter() const;
	bool isDoor() const;
	bool isWall() const;

private:
	TileType type;
	AnimationType animType;
	Animation* anim;

	uint x;
	uint y;

};

#endif//TILE_H
