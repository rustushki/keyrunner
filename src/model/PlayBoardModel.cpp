#include "../model/BoardModel.hpp"
#include "PlayBoardModel.hpp"

PlayBoardModel::PlayBoardModel() {
    playerHasKey = false;
}

/**
 * Is the currently loaded level finished?
 * <p>
 * This happens when the player has the key and is standing at the door.
 * @return boolean
 */
bool PlayBoardModel::isComplete() const {
    return (playerHasKey && isInDoor(getPlayerCoord()));
}

/**
 * Move the player in the provided direction.
 * @param direction
 */
void PlayBoardModel::movePlayerInDirection(Direction direction) {

    Coordinate newCoordinate = getCoordinateInDirection(getPlayerCoord(), direction);

    // Do not move player if the new tile is a wall. Do not continue evaluating criteria either, such as teleporters
    // and wraparound. They do not apply since the player has attempted to walk into a wall
    if (isInWall(newCoordinate)) {
        return;
    }

    // Give the player the key if the tile has the key
    if (!getPlayerHasKey()) {
        if (getKey()->intersectsWithCoordinate(newCoordinate)) {
            setPlayerHasKey(true);
        }
    }

    // Move the player to the coordinate
    setPlayerCoord(newCoordinate);
}

/**
 * Get the time remaining on the time clock.
 * @return long
 */
long PlayBoardModel::getTimeClock() {
    return timeClock;
}

/**
 * Set the time remaining on the time clock.
 * @param uint16_t
 */
void PlayBoardModel::setTimeClock(long timeClockValue) {
    timeClock = timeClockValue;
}

/**
 * Decrement the time clock by a certain amount.
 * @param step
 */
void PlayBoardModel::decrementTimeClock(long step) {
    timeClock -= step;
}

/**
 * Increment the time clock by a certain amount.
 * @param step
 */
void PlayBoardModel::incrementTimeClock(long step) {
    timeClock += step;
}

/**
 * Set if the player has the key.
 * @param playerHasKey
 */
void PlayBoardModel::setPlayerHasKey(bool playerHasKey) {
    this->playerHasKey = playerHasKey;
    if (this->playerHasKey) {
        setKeyCoord(Coordinate(getWidth(), getHeight()));
    }
}

/**
 * Convey the player in the direction of the conveyor tile on which they stand.
 * <p>
 * If the player is not on a conveyor tile, this is a non-operation.
 */
void PlayBoardModel::conveyPlayer() {
    // Convert the player coordinate into a tile coordinate
    TileCoordinate tileHoldingPlayer = coordinateToTileCoordinate(getPlayerCoord());

    if (isConveyor(tileHoldingPlayer)) {
        Coordinate newCoordinate = getNextConveyorCoordinate(getPlayerCoord());

        if (!getPlayerHasKey()) {
            if (getKey()->intersectsWithCoordinate(newCoordinate)) {
                setPlayerHasKey(true);
            }
        }

        setPlayerCoord(newCoordinate);
    }
}

bool PlayBoardModel::getPlayerHasKey() {
    return playerHasKey;
}
