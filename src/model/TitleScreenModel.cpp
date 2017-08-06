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
