#include "../model/BaseBoardEntity.hpp"

/**
 * Constructor.
 */
BaseBoardEntity::BaseBoardEntity(Coordinate coordinate, BoardEntityType type) : coordinate{coordinate}, type{type} {}

/**
 * Retrieves the hit boxes associated with the BoardEntity.
 * @return std::vector<HitBox*>
 */
std::vector<HitBox*>& BaseBoardEntity::getHitBoxes() {
    return hitBoxes;
}

/**
 * Set the screen location of the BoardEntity.
 * <p>
 * This is the coordinate of the top left corner of the board entity as it appears on the screen.
 * @param coordinate
 */
void BaseBoardEntity::setCoordinate(Coordinate coordinate) {
    this->coordinate = coordinate;

    for (HitBox* hitBox : getHitBoxes()) {
        hitBox->setAnchor(coordinate);
    }
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
 * @return boolean
 */
bool BaseBoardEntity::intersectsWithCoordinate(Coordinate coordinate) {
    bool intersects = false;
    for (HitBox* hitBox : getHitBoxes()) {
        if (hitBox->contains(coordinate)) {
            intersects = true;
            break;
        }

    }
    return intersects;
}

/**
 * Check to see if the provided board entity intersects with this one.
 * @param entity
 * @return boolean
 */
bool BaseBoardEntity::intersectsWithEntity(BoardEntity* entity) {
    bool intersects = false;
    for (HitBox* myHitBox : getHitBoxes()) {
        for (HitBox* itsHitBox : entity->getHitBoxes()) {
            if (myHitBox->intersects(itsHitBox)) {
                intersects = true;
                break;
            }
        }

        if (intersects) {
            break;
        }
    }

    return intersects;
}

/**
 * Set the type of board entity.
 * @param type
 */
void BaseBoardEntity::setType(BoardEntityType type) {
    this->type = type;
}

