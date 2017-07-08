#include "EditorBoardModel.hpp"

/**
 * Constructor.
 */
EditorBoardModel::EditorBoardModel() {
    this->tileType = TILE_TYPE_EMPTY;
}

/**
 * Sets the tile type that will be used when placing tiles on the board.
 * @param tileType
 */
void EditorBoardModel::setTileType(TileType tileType) {
    this->tileType = tileType;
}

/**
 * Gets the tile type that will be used when placing tiles on the board.
 * @return
 */
TileType EditorBoardModel::getTileType() const {
    return this->tileType;
}
