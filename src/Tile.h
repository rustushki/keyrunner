#ifndef TILE_H
#define TILE_H

#include <vector>
#include "TileType.h"
#include "AnimationType.h"
class Animation;

class Tile {

public:
	Tile(TileType type, uint x, uint y, Level*);
	~Tile();

	void draw();

	Animation* getAnimation() const;
	static AnimationType TileTypeToAnimType(TileType tt);
	static void PushAnimatedTile(Tile* tile);
	static void AnimateTiles();
	static void ClearAnimatedTiles();
	static void RedrawChangedTiles();
	static void AddChangedTile(Tile* tile);

	TileType getType() const;

	uint getX() const;
	uint getY() const;

	bool isTeleporter() const;
	bool isDoor() const;
	bool isWall() const;
	bool hasPlayer() const;
	bool hasKey() const;

private:
	TileType type;
	AnimationType animType;
	Animation* anim;
	Level* level;

	static std::vector<Tile*> ChangedTiles;
	static std::vector<Tile*> AnimatedTiles;

	uint x;
	uint y;

};

#endif//TILE_H
