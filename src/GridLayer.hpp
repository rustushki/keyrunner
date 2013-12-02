#ifndef GLIDLAYER_HPP
#define GLIDLAYER_HPP

#include <vector>

class Tile;

class GridLayer {

public:

	static GridLayer* GetInstance();

	void pushAnimatedTile(Tile* tile); 
	void clearAnimatedTiles();
	void animateTiles();
	void redrawChangedTiles();
	void clearChangedTiles();
	void addChangedTile(Tile* tile);

private:

	static GridLayer* instance;

	GridLayer();

	std::vector<Tile*> changedTiles;
	std::vector<Tile*> animatedTiles;

};

#endif//GLIDLAYER_HPP
