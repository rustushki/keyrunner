#ifndef BASE_BOARD_ENTITY_HPP
#define BASE_BOARD_ENTITY_HPP

#include "../model/BoardEntity.hpp"

class BaseBoardEntity : public BoardEntity {
public:
    BaseBoardEntity();

    std::vector<HitBox*> getHitBoxes() override;
    bool intersectsWithEntity(BoardEntity* entity) override;
    bool intersectsWithCoordinate(Coordinate coordinate) override;
    Coordinate getCoordinate() override;
    void setCoordinate(Coordinate coordinate) override;
    BoardEntityType getType() override;
    void setType(BoardEntityType type) override;

private:
    BoardEntityType type;
    Coordinate coordinate;
};

#endif
