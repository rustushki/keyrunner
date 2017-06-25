#ifndef EDITOR_MODEL_HPP
#define EDITOR_MODEL_HPP

#include "../model/TileType.hpp"
#include "ButtonModel.hpp"

class EditorModel {
    public:

        // Garbage
        static EditorModel* GetInstance();
        void setTileType(TileType tileType);
        TileType getTileType() const;

    private:
        ButtonModel* exit;

        // Garbage
        static EditorModel* instance;
        EditorModel();
        TileType tileType;
};

#endif
