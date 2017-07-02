#include "../model/EditorModel.hpp"

/**
 * Constructor.
 * <p>
 * Initialize the Editor's fields.
 */
EditorModel::EditorModel() : BaseModel() {
    this->tileType = TILE_TYPE_EMPTY;
}

/**
 * Set Tile Type.
 * <p>
 * This is the tile type which is used by the editor when the user presses
 * Enter on the grid.
 */
void EditorModel::setTileType(TileType tileType) {
    this->tileType = tileType;
}

/**
 * Get Tile Type.
 * <p>
 * This is the tile type which is used by the editor when the user presses
 * Enter on the grid.
 */
TileType EditorModel::getTileType() const {
    return tileType;
}
