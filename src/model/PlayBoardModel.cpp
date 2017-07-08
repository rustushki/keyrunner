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
    return (playerHasKey && isDoor(getPlayerCoord()));
}

/**
 * Move the player in the provided direction.
 * @param direction
 */
void PlayBoardModel::movePlayerInDirection(Direction direction) {
    TileCoord oldTileCoord = getPlayerCoord();
    TileCoord newTileCoord;
    if (direction == DIRECTION_UP) {
        newTileCoord = getTileCoordUp(oldTileCoord);
    }

    if (direction == DIRECTION_DOWN) {
        newTileCoord = getTileCoordDown(oldTileCoord);
    }

    if (direction == DIRECTION_LEFT) {
        newTileCoord = getTileCoordLeft(oldTileCoord);
    }

    if (direction == DIRECTION_RIGHT) {
        newTileCoord = getTileCoordRight(oldTileCoord);
    }

    // Do not move player if the new tile is a wall. Do not continue evaluating criteria either, such as teleporters
    // and wraparound. They do not apply since the player has attempt to walk into a wall
    if (isWall(newTileCoord)) {
        return;
    }

    // Give the player the key if the tile has the key
    if (tileCoordHasKey(newTileCoord)) {
        setPlayerHasKey(true);
    }

    // Move the player to the tile
    setPlayerCoord(newTileCoord);
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
        setKeyCoord(TileCoord(getWidth(), getHeight()));
    }
}

/**
 * Convey the player to the next conveyor tile in the belt.
 * <p>
 * If the player is not on a conveyor tile, this is a non-operation.
 */
void PlayBoardModel::conveyPlayer() {
    // If the player is on a conveyor tile, convey the player to the next tile on the belt
    if (isConveyor(getPlayerCoord())) {
        TileCoord newTileCoord = getNextConveyorTileCoord(getPlayerCoord());
        setPlayerCoord(newTileCoord);

        if (tileCoordHasKey(newTileCoord)) {
            setPlayerHasKey(true);
        }
    }
}
