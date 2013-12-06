#ifndef GLIDLAYER_HPP
#define GLIDLAYER_HPP

#include <vector>

#include "Layer.hpp"

class Tile;

class GridLayer : public Layer {

public:

	static GridLayer* GetInstance();

	void pushAnimatedTile(Tile* tile); 
	void clearAnimatedTiles();
	void animateTiles();
	void redrawChangedTiles();
	void clearChangedTiles();
	void addChangedTile(Tile* tile);

	void draw(SDL_Surface* dst);

private:

	static GridLayer* instance;

	GridLayer();

	std::vector<Tile*> changedTiles;
	std::vector<Tile*> animatedTiles;

};

#endif//GLIDLAYER_HPP
