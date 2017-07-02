#ifndef EDITOR_MODEL_HPP
#define EDITOR_MODEL_HPP

#include "../model/TileType.hpp"
#include "../model/BaseModel.hpp"

class EditorModel : public BaseModel {
public:
    EditorModel();

    // Garbage
    void setTileType(TileType tileType);
    TileType getTileType() const;

private:
    // Garbage
    TileType tileType;
};

#endif
