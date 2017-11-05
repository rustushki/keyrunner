#ifndef BOARD_ENTITY_HPP
#define BOARD_ENTITY_HPP

#include <cstdint>
#include <vector>
#include "../model/BoardEntityType.hpp"
#include "../model/HitBox.hpp"
#include "../uitk/SpriteSheet.hpp"
#include "../uitk/AnimationData.hpp"

class BoardEntity {
public:
    virtual std::vector<HitBox*> getHitBoxes() = 0;
    virtual bool intersects(BoardEntity* entity) = 0;
    virtual uint32_t getBoardX() = 0;
    virtual uint32_t getBoardY() = 0;
    virtual BoardEntityType getType() = 0;
};

#endif
