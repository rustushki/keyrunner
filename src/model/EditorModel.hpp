#ifndef EDITOR_MODEL_HPP
#define EDITOR_MODEL_HPP

#include "../model/TileType.hpp"
#include "../model/BaseModel.hpp"

class EditorModel : public BaseModel {
    public:
        // Garbage
        static EditorModel* GetInstance();
        void setTileType(TileType tileType);
        TileType getTileType() const;

    private:
        // Garbage
        static EditorModel* instance;
        EditorModel();
        TileType tileType;
};

#endif
