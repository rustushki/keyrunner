#ifndef TITLE_SCREEN_MODEL_HPP
#define TITLE_SCREEN_MODEL_HPP

#include "../model/BaseModel.hpp"
#include "../model/LevelManager.hpp"

class TitleScreenModel : public BaseModel {
public:
    TitleScreenModel();
    uint16_t getMaxLevel() const;

private:
    LevelManager* levelManager;

};

#endif
