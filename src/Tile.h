#ifndef TILE_H
#define TILE_H

#include <vector>
#include "Animation.h"
#include "AnimationType.h"
#include "Direction.h"
#include "TileType.h"

class Level;

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
	bool isConveyor() const;
	bool hasPlayer() const;
	bool hasKey() const;

	Tile* getNextConveyorTile() const;

	Tile* up() const;
	Tile* down() const;
	Tile* left() const;
	Tile* right() const;

	Tile* getTileInDirection(Direction d) const;

	Direction getConveyorDirection() const;

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
