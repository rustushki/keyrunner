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
    this->optionModel = new OptionModel();
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
    delete this->optionModel;
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
 * Fetch the option model
 * @return OptionModel*
 */
OptionModel *GameModel::getOptionModel() const {
    return this->optionModel;
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
