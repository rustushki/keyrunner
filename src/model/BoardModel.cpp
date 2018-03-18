#include <stdexcept>
#include <sstream>
#include "../model/BoardModel.hpp"
#include "../model/TileCoordinate.hpp"

/**
 * Constructor.
 * <p>
 * Initializes the tile type grid to be sized as getGridHeight() and getGridWidth() indicate.
 */
BoardModel::BoardModel() : playerHasKey(false) {
    for (int row = 0; row < getHeight(); row++) {
        std::vector<TileType> tileRow;
        for (int column = 0; column < getWidth(); column++) {
            tileRow.push_back(TileType::Empty);
        }
        this->tileType.push_back(tileRow);
    }
}

/**
 * Destructor.
 */
BoardModel::~BoardModel() {
    clearBoardEntities();
}

/**
 * Get the level number of the currently loaded level.
 * @return uint8_t
 */
uint8_t BoardModel::getLevelNum() const {
    return level;
}

/**
 * Set the number of the currently loaded level.
 * @param level
 */
void BoardModel::setLevelNum(uint8_t level) {
    this->level = level;
}

/**
 * Does the provided hit box intersect with a wall?
 * @param coord
 * @return boolean
 */
bool BoardModel::isHitBoxInWall(HitBox* hitBox) {
    bool isInWall = false;
    for (HitBox* wall : getWallHitBoxes()) {
        if (wall->intersects(hitBox)) {
            isInWall = true;
            break;
        }
    }

    return isInWall;
}

/**
 * Get the coordinate of the key.
 * @return BoardEntityCoordinate
 */
Coordinate BoardModel::getKeyCoord() const {
    auto matchingEntityVector = getEntityByType(KEY);
    if (matchingEntityVector.empty()) {
        throw std::logic_error("No key found on board");
    }
    Coordinate match = matchingEntityVector.front()->getCoordinate();

    return match;
}

/**
 * Set the coordinate of the key.
 * @param coordinate
 */
void BoardModel::setKeyCoord(Coordinate coordinate) {
    auto matchingEntityVector = getEntityByType(KEY);
    if (matchingEntityVector.empty()) {
        throw std::logic_error("No key found on board");
    }
    matchingEntityVector.front()->setCoordinate(coordinate);
}

/**
 * Search through the list of loaded board entities and find the ones that match the provided type.
 * @param type
 * @return std::vector<BoardEntity*>
 */
std::vector<BoardEntity*> BoardModel::getEntityByType(BoardEntityType type) const {
    std::vector<BoardEntity*> entityVector;
    for (BoardEntity* entity : getBoardEntities()) {
        if (entity->getType() == type) {
            entityVector.push_back(entity);
            break;
        }
    }

    return entityVector;
}

/**
 * Get the current coordinate of the player.
 * @return BoardEntityCoordinate
 */
Coordinate BoardModel::getPlayerCoord() const {
    return getPlayer()->getCoordinate();
}

/**
 * Assign a new coordinate to the player.
 * <p>
 * If the coordinate is a teleporter, move the player to the matching teleporter's coordinates.
 * @param coordinate
 */
void BoardModel::setPlayerCoord(Coordinate coordinate) {
    BoardEntity* player = getPlayer();

    for (BoardEntity* entity : getBoardEntities()) {
        if (player->intersectsWithEntity(entity)) {
            if (isTeleporter(entity)) {
                coordinate = getMatchingTeleporter(entity)->getCoordinate();
                break;
            }
        }
    }

    player->setCoordinate(coordinate);
}

/**
 * Is the provided entity a teleporter?
 * @param BoardEntity
 * @return boolean
 */
bool BoardModel::isTeleporter(BoardEntity* entity) const {
    BoardEntityType type = entity->getType();

    return (   type == TELEPORTER_RED
            || type == TELEPORTER_GREEN
            || type == TELEPORTER_BLUE);
}

/**
 * Change the tile type of the tile at the provided tile coordinate.
 * @param tileCoordinate
 * @param newTileType
 */
void BoardModel::changeTileType(TileCoordinate tileCoordinate, TileType newTileType) {
    tileType[tileCoordinate.getY()][tileCoordinate.getX()] = newTileType;
}

/**
 * Get the type of the tile at the provided coordinate.
 * @param tileCoordinate
 * @return TileType
 */
TileType BoardModel::getTileType(TileCoordinate tileCoordinate) const {
    return tileType[tileCoordinate.getY()][tileCoordinate.getX()];
}

/**
 * Is the Tile containing the provided coordinate a Door?
 * @param coord
 * @return boolean
 */
bool BoardModel::isInDoor(Coordinate coordinate) const {
    TileCoordinate tileCoordinate(coordinate);
    return tileType[tileCoordinate.getY()][tileCoordinate.getX()] == TileType::Door;
}

/**
 * Return true if the tile coordinate has a conveyor tile type.
 * @param coord
 * @return boolean
 */
bool BoardModel::isConveyor(TileCoordinate coord) const {
    bool isConveyor;

    if (coord.getX() < 0 || coord.getY() < 0 || coord.getX() >= getWidth() || coord.getY() >= getHeight()) {
        isConveyor = false;
    } else {
        TileType tt = tileType[coord.getY()][coord.getX()];

        isConveyor = (tt == TileType::ConveyorUp || tt == TileType::ConveyorDown || tt == TileType::ConveyorRight
                || tt == TileType::ConveyorLeft);
    }

    return isConveyor;
}

/**
 * Return the direction the conveyor is pointing towards.
 * @param coord
 * @return Direction
 * @throws std::invalid_argument if tile coordinate is not for conveyor tile
 */
Direction BoardModel::getConveyorDirection(TileCoordinate coord) const {
    TileType tt = tileType[coord.getY()][coord.getX()];
    if (tt == TileType::ConveyorUp) {
        return DIRECTION_UP;
    } else if (tt == TileType::ConveyorDown) {
        return DIRECTION_DOWN;
    } else if (tt == TileType::ConveyorRight) {
        return DIRECTION_RIGHT;
    } else if (tt == TileType::ConveyorLeft) {
        return DIRECTION_LEFT;
    }

    std::stringstream errorMessage;
    errorMessage << "Non-conveyor tile queried for direction.";
    throw std::invalid_argument(errorMessage.str());
}

/**
 * Given a teleporter, return the matching teleporter.
 * @param teleporter
 * @return BoardEntity
 */
BoardEntity* BoardModel::getMatchingTeleporter(BoardEntity* teleporter) const {
    if (!isTeleporter(teleporter)) {
        throw std::invalid_argument("Expected teleporter");
    }

    BoardEntity* matchingTeleporter = nullptr;
    BoardEntityType teleporterType = teleporter->getType();
    for (BoardEntity* entity : getBoardEntities()) {
        if (entity->getType() == teleporterType && entity != teleporter) {
            matchingTeleporter = entity;
            break;
        }
    }

    if (matchingTeleporter == nullptr) {
        throw std::logic_error("Could not find matching teleporter");
    }

    return matchingTeleporter;
}

/**
 * Get the Board Height.
 * @return uint16_t
 */
uint16_t BoardModel::getHeight() const {
    return 16;
}

/**
 * Get the Board Width.
 * @return uint16_t
 */
uint16_t BoardModel::getWidth() const {
    return 25;
}

/**
 * Get the list of loaded board entities.
 * @return std::vector<BoardEntity*>
 */
std::vector<BoardEntity*> BoardModel::getBoardEntities() const {
    return this->boardEntities;
}

/**
 * Search through the list of board entities and return the one that is the player.
 * <p>
 * It is a logic error if there is not a player.
 * @return
 */
BoardEntity* BoardModel::getPlayer() const {
    auto matchingEntityVector = getEntityByType(PLAYER);
    if (matchingEntityVector.empty()) {
        throw std::logic_error("No player found on board");
    }
    return matchingEntityVector.front();
}

/**
 * Search through the list of board entities and return the one that is the key.
 * @return
 */
BoardEntity* BoardModel::getKey() const {
    auto matchingEntityVector = getEntityByType(KEY);
    return matchingEntityVector.front();
}

/**
 * Return the list of wall hit boxes.
 * <p>
 * Note that the walls are just invisible hit boxes.
 * @return
 */
std::vector<HitBox*>& BoardModel::getWallHitBoxes() {
    return wallHitBoxes;
}

/**
 * Get the coordinate 5 pixels in the provided direction.
 * @param startingCoordinate
 * @param direction
 * @return Coordinate
 */
Coordinate BoardModel::getCoordinateInDirection(Coordinate startingCoordinate, Direction direction) const {
    Coordinate newCoordinate{0, 0};

    if (direction == DIRECTION_UP) {
        newCoordinate = {startingCoordinate.getX(), startingCoordinate.getY() - 5};
    }

    if (direction == DIRECTION_DOWN) {
        newCoordinate = {startingCoordinate.getX(), startingCoordinate.getY() + 5};
    }

    if (direction == DIRECTION_LEFT) {
        newCoordinate = {startingCoordinate.getX() - 5, startingCoordinate.getY()};
    }

    if (direction == DIRECTION_RIGHT) {
        newCoordinate = {startingCoordinate.getX() + 5, startingCoordinate.getY()};
    }

    return newCoordinate;
}

/**
 * Replace the list of BoardEntity objects currently on the board.
 * @param entities
 */
void BoardModel::setBoardEntities(std::vector<BoardEntity*> entities) {
    clearBoardEntities();
    this->boardEntities = std::move(entities);
}

/**
 * Set if the player has the key.
 * @param playerHasKey
 */
void BoardModel::setPlayerHasKey(bool playerHasKey) {
    this->playerHasKey = playerHasKey;
}

/**
 * Return whether or not the player current has the key.
 * @return boolean
 */
bool BoardModel::getPlayerHasKey() const {
    return playerHasKey;
}

/**
 * Clears the list of loaded board entities and frees the memory associated with each.
 */
void BoardModel::clearBoardEntities() {
    for (BoardEntity* entity : getBoardEntities()) {
        delete entity;
    }

    boardEntities.clear();
}
