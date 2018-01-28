#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <vector>
#include "../model/BaseModel.hpp"
#include "../model/BoardEntity.hpp"
#include "../model/Direction.hpp"
#include "../model/TileType.hpp"

class BoardModel : public BaseModel {
public:
    BoardModel();
    ~BoardModel();

    uint16_t getHeight() const;
    uint16_t getWidth() const;

    uint8_t getLevelNum() const;
    void setLevelNum(uint8_t level);

    Coordinate getKeyCoord() const;
    void setKeyCoord(Coordinate coordinate);
    BoardEntity* getKey() const;

    Coordinate getPlayerCoord() const;
    void setPlayerCoord(Coordinate coordinate);
    BoardEntity* getPlayer() const;

    void setPlayerHasKey(bool playerHasKey);
    bool getPlayerHasKey() const;

    bool isHitBoxInWall(HitBox* hitBox);
    bool isTeleporter(BoardEntity* entity) const;
    bool isInDoor(Coordinate coordinate) const;
    bool isConveyor(TileCoordinate coord) const;

    TileType getTileType(TileCoordinate tileCoordinate) const;
    void changeTileType(TileCoordinate tileCoordinate, TileType newTileType);

    Direction getConveyorDirection(TileCoordinate coord) const;

    BoardEntity* getMatchingTeleporter(BoardEntity* teleporter) const;

    std::vector<BoardEntity*> getBoardEntities() const;

    Coordinate getCoordinateInDirection(Coordinate startingCoordinate, Direction direction) const;

    void setBoardEntities(std::vector<BoardEntity*> vector);

    std::vector<HitBox*>& getWallHitBoxes();

private:
    uint8_t level;
    std::vector< std::vector<TileType> > tileType;
    std::vector<HitBox*> wallHitBoxes;
    std::vector<BoardEntity*> boardEntities;
    bool playerHasKey;

    std::vector<BoardEntity*> getEntityByType(BoardEntityType type) const;
    void clearBoardEntities();
};

#endif
