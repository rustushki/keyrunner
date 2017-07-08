#ifndef EDITOR_BOARD_MODEL_HPP
#define EDITOR_BOARD_MODEL_HPP

#include "BoardModel.hpp"

class EditorBoardModel : public BoardModel {
public:
    EditorBoardModel();
    void setTileType(TileType tileType);
    TileType getTileType() const;
    TileCoord getHoverTileCoordinate() const;
    void setHoverTileCoordinate(TileCoord newHoverCoordinate);

private:
    TileType tileType;
    TileCoord hoverCoordinate;
};

#endif
