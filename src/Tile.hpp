#ifndef TILE_HPP
#define TILE_HPP

#include <vector>
#include "Animation.hpp"
#include "AnimationType.hpp"
#include "Direction.hpp"
#include "TileType.hpp"

class Level;

class Tile {

public:
	Tile(TileType type, uint16_t x, uint16_t y, Level*);
	~Tile();

	void draw();

	Animation* getAnimation() const;
	static AnimationType TileTypeToAnimType(TileType tt);
	static void PushAnimatedTile(Tile* tile);
	static void AnimateTiles();
	static void ClearAnimatedTiles();
	static void RedrawChangedTiles();
	static void AddChangedTile(Tile* tile);
	static void ClearChangedTiles();

	TileType getType() const;

	uint16_t getX() const;
	uint16_t getY() const;

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

	uint16_t x;
	uint16_t y;

};

#endif//TILE_HPP
