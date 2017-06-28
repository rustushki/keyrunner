#include "../model/EditorModel.hpp"

/**
 * Singleton Instance of EditorModel.
 */
EditorModel* EditorModel::instance = NULL;

/**
 * Get Singleton Instance.
 * <p>
 * This should be removed soon.
 */
EditorModel* EditorModel::GetInstance() {
    if (EditorModel::instance == NULL) {
        EditorModel::instance = new EditorModel();
    }

    return EditorModel::instance;
}

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
