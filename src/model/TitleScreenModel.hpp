#ifndef TITLE_SCREEN_MODEL_HPP
#define TITLE_SCREEN_MODEL_HPP

#include "../model/BaseModel.hpp"
#include "../model/LevelManager.hpp"

class TitleScreenModel : public BaseModel {
public:
    TitleScreenModel();
    uint16_t getMaxLevel() const;
    void setCreateNewLevel(bool createNewLevel);
    void setEditorLevel(uint8_t editorLevel);
    uint8_t getEditorLevel() const;
    bool getCreateNewLevel() const;

private:
    LevelManager* levelManager;
    bool createNewLevel;
    uint8_t editorLevel;
};

#endif
