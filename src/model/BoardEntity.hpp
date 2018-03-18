#ifndef BOARD_ENTITY_HPP
#define BOARD_ENTITY_HPP

#include <cstdint>
#include <vector>
#include "../model/BoardEntityType.hpp"
#include "../model/HitBox.hpp"
#include "../uitk/SpriteSheet.hpp"

class BoardEntity {
public:
    virtual std::vector<HitBox*>& getHitBoxes() = 0;
    virtual bool intersectsWithEntity(BoardEntity* entity) = 0;
    virtual bool intersectsWithCoordinate(Coordinate coordinate) = 0;
    virtual Coordinate getCoordinate() = 0;
    virtual void setCoordinate(Coordinate coordinate) = 0;
    virtual BoardEntityType getType() = 0;
    virtual void setType(BoardEntityType type) = 0;
};

#endif
