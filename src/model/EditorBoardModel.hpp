#ifndef EDITOR_BOARD_MODEL_HPP
#define EDITOR_BOARD_MODEL_HPP

#include "../model/BoardModel.hpp"
#include "../model/HoverBehavior.hpp"
#include "../model/TileCoordinate.hpp"

class EditorBoardModel : public BoardModel {
public:
    EditorBoardModel();
    void setTileType(TileType tileType);
    TileType getTileType() const;
    TileCoordinate getHoverTileCoordinate() const;
    void setHoverTileCoordinate(TileCoordinate newHoverCoordinate);
    HoverBehavior getHoverBehavior();
    void setHoverBehavior(HoverBehavior newBehavior);

private:
    TileType tileType;
    TileCoordinate hoverCoordinate{0, 0};
    HoverBehavior hoverBehavior;
};

#endif
