#include "BaseBoardEntity.hpp"

/**
 * Constructor.
 */
BaseBoardEntity::BaseBoardEntity() : coordinate{0, 0} {}

/**
 * Retrieves the hitboxes associated with the BoardEntity.
 * <p>
 * Currently unimplemented.
 * @return
 */
std::vector<HitBox*> BaseBoardEntity::getHitBoxes() {
    return std::vector<HitBox*>();
}

/**
 * Set the screen location of the BoardEntity.
 * <p>
 * This is the coordinate of the top left corner of the board entity as it appears on the screen.
 * @param coordinate
 */
void BaseBoardEntity::setCoordinate(Coordinate coordinate) {
    this->coordinate = coordinate;
}

/**
 * Get the type of board entity.
 * @return BoardEntityType
 */
BoardEntityType BaseBoardEntity::getType() {
    return type;
}

/**
 * Get the screen location of the BoardEntity.
 * <p>
 * This is the coordinate of the top left corner of the board entity as it appears on the screen.
 * @return Coordinate
 */
Coordinate BaseBoardEntity::getCoordinate() {
    return this->coordinate;
}

/**
 * Check to see if the provided coordinate falls within the bounds of the board entity.
 * <p>
 * Internally, this merely checks all the hit boxes to see if the coordinate intersects with any of them.
 * @param coordinate
 * @return
 */
bool BaseBoardEntity::intersectsWithCoordinate(Coordinate coordinate) {
    return false;
}

/**
 * Check to see if the provided board entity intersects with this one.
 * @param entity
 * @return boolean
 */
bool BaseBoardEntity::intersectsWithEntity(BoardEntity* entity) {
    return false;
}

/**
 * Set the type of board entity.
 * @param type
 */
void BaseBoardEntity::setType(BoardEntityType type) {
    this->type = type;
}

