#include "GameModel.hpp"

/**
 * Constructor.
 * <p>
 * Initializes all models for the game.
 */
GameModel::GameModel() {
    this->titleScreenModel = new TitleScreenModel();
    this->editorBoardModel = new EditorBoardModel();
    this->playBoardModel = new PlayBoardModel();
}

/**
 * Destructor.
 * <p>
 * Destroys all models for the game.
 */
GameModel::~GameModel() {
    delete this->titleScreenModel;
    delete this->editorBoardModel;
    delete this->playBoardModel;
}

/**
 * Fetch the Title Screen's model.
 * @return TitleScreenModel*
 */
TitleScreenModel *GameModel::getTitleScreenModel() const {
    return this->titleScreenModel;
}

/**
 * Fetch the Editor's model.
 * @return EditorBoardModel*
 */
EditorBoardModel *GameModel::getEditorBoardModel() const {
    return this->editorBoardModel;
}

/**
 * Fetch the Play model.
 * @return PlayBoardModel*
 */
PlayBoardModel *GameModel::getPlayBoardModel() const {
    return this->playBoardModel;
}

/**
 * Get the state of the game.
 * @return State
 */
State GameModel::getState() const {
    return this->state;
}

/**
 * Set the state of the game.
 * @param state
 */
void GameModel::setState(State state) {
    this->state = state;
}
