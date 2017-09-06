#include "TitleScreenModel.hpp"

/**
 * Constructor.
 */
TitleScreenModel::TitleScreenModel() {
    levelManager = {};
}

/**
 * Return the maximum count of levels.
 * @return uint16_t
 */
uint16_t TitleScreenModel::getMaxLevel() const {
    return levelManager->getLevelCount();
}

/**
 * Set whether or not the editor should create a new level when started.
 * @param createNewLevel
 */
void TitleScreenModel::setCreateNewLevel(bool createNewLevel) {
    this->createNewLevel = createNewLevel;
}

/**
 * Set the level number that the editor should load.
 * @param editorLevel
 */
void TitleScreenModel::setEditorLevel(uint8_t editorLevel) {
    this->editorLevel = editorLevel;
}

/**
 * Get the level number that the editor should load.
 * @return
 */
uint8_t TitleScreenModel::getEditorLevel() const {
    return editorLevel;
}

/**
 * Get whether or not the editor should create a new level when started.
 * @return
 */
bool TitleScreenModel::getCreateNewLevel() const {
    return createNewLevel;
}
