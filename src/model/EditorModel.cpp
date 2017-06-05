#include <stddef.h>
#include "EditorModel.hpp"

/* ------------------------------------------------------------------------------
 * Singleton Instance of EditorModel.
 */
EditorModel* EditorModel::instance = NULL;

/* ------------------------------------------------------------------------------
 * Get Singleton Instance.
 *
 * Make the EditorModel singleton so that it can only be created once and so
 * that that instance may be accessed anywhere in the view or controller.
 */
EditorModel* EditorModel::GetInstance() {
    if (EditorModel::instance == NULL) {
        EditorModel::instance = new EditorModel();
    }

    return EditorModel::instance;
}

/* ------------------------------------------------------------------------------
 * Constructor.
 *
 * Initialize the Editor's fields.
 */
EditorModel::EditorModel() : tileType{TILE_TYPE_EMPTY} {}

/* ------------------------------------------------------------------------------
 * Set Tile Type.
 *
 * This is the tile type which is used by the editor when the user presses
 * Enter on the grid.
 */
void EditorModel::setTileType(TileType tileType) {
    this->tileType = tileType;
}

/* ------------------------------------------------------------------------------
 * Get Tile Type.
 *
 * This is the tile type which is used by the editor when the user presses
 * Enter on the grid.
 */
TileType EditorModel::getTileType() const {
    return tileType;
}
