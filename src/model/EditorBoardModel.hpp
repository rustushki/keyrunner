#ifndef EDITOR_BOARD_MODEL_HPP
#define EDITOR_BOARD_MODEL_HPP

#include "../model/BoardModel.hpp"
#include "../model/HoverBehavior.hpp"

class EditorBoardModel : public BoardModel {
public:
    EditorBoardModel();
    void setTileType(TileType tileType);
    TileType getTileType() const;
    TileCoord getHoverTileCoordinate() const;
    void setHoverTileCoordinate(TileCoord newHoverCoordinate);
    HoverBehavior getHoverBehavior();
    void setHoverBehavior(HoverBehavior newBehavior);

private:
    TileType tileType;
    TileCoord hoverCoordinate;
    HoverBehavior hoverBehavior;
};

#endif
