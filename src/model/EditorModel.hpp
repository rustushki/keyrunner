#ifndef EDITORMODEL_HPP
#define EDITORMODEL_HPP

#include "../game/TileType.hpp"

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

#endif//EDITORMODEL_HPP
