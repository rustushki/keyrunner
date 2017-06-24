#ifndef EDITOR_MODEL_HPP
#define EDITOR_MODEL_HPP

#include "../model/TileType.hpp"

class EditorModel {
    public:
        static EditorModel* GetInstance();

        void setTileType(TileType tileType);
        TileType getTileType() const;

    private:
        static EditorModel* instance;

        EditorModel();
        TileType tileType;
};

#endif
