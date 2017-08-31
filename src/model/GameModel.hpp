#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include "../model/Model.hpp"
#include "../model/TitleScreenModel.hpp"
#include "../model/EditorBoardModel.hpp"
#include "../model/OptionModel.hpp"
#include "../model/PlayBoardModel.hpp"
#include "../model/GameOverModel.hpp"

class GameModel : public Model {
public:
    GameModel();
    virtual ~GameModel();

    State getState() const final;
    void setState(State state) final;
    TitleScreenModel* getTitleScreenModel() const;
    EditorBoardModel* getEditorBoardModel() const;
    PlayBoardModel* getPlayBoardModel() const;
    OptionModel* getOptionModel() const;
    GameOverModel *getGameOverModel() const;

private:
    TitleScreenModel* titleScreenModel;
    EditorBoardModel* editorBoardModel;
    PlayBoardModel* playBoardModel;
    OptionModel* optionModel;
    GameOverModel* gameOverModel;
    State state;
};

#endif
