#include <sstream>

#include "../controller/KeyRunner.hpp"
#include "../model/BaseBoardEntity.hpp"
#include "../model/LevelManager.hpp"
#include "../model/TileCoordinate.hpp"
#include "RectangleHitBox.hpp"

/**
 * Constructor.
 * <p>
 * Builds a LevelManager which is connected to the provided BoardModel.
 * @param board
 */
LevelManager::LevelManager(BoardModel* board) : keyCoordinate{0, 0}, playerCoordinate{0, 0} {
    this->board = board;
}

/**
 * Get the total count of levels available for loading.
 * @return uint8_t
 */
uint8_t LevelManager::getLevelCount() const {
    // Compute and statically cache the maximum count of levels for loading
    static int levelCount = -1;
    if (levelCount == -1) {
        // Perform a binary search to determine the maximum number of levels available for loading.
        uint8_t hiLevel = 255;
        uint8_t loLevel = 0;
        uint8_t miLevel = 0;

        while (hiLevel >= loLevel) {
            miLevel = (uint8_t) ((hiLevel - loLevel) / 2 + loLevel);
            if (levelExists(miLevel) && !levelExists(static_cast<uint8_t>(miLevel + 1))) {
                levelCount = miLevel;
                break;
            }

            if (levelExists(miLevel)) {
                loLevel = static_cast<uint8_t>(miLevel + 1);
            } else {
                hiLevel = static_cast<uint8_t>(miLevel - 1);
            }
        }

        levelCount = miLevel;
    }

    return static_cast<uint8_t>(levelCount);
}

/**
 * Return true if the given level number corresponds with a level file that actually exists.
 * @param levelNumber
 * @return boolean
 */
bool LevelManager::levelExists(uint8_t levelNumber) const {
    std::string levelFile = getPath(levelNumber, false);
    FILE* fp = fopen(levelFile.c_str(), "rb");

    bool exists = false;
    if (fp != nullptr) {
        exists = true;
        fclose(fp);
    }

    return exists;
}

/**
 * Writes the current state of the board into a binary-packed, persistently stored level file.
 */
void LevelManager::write() const {
    std::string levelFile = getPath(board->getLevelNum(), true);
    FILE* fp = fopen(levelFile.c_str(), "wb");
    writeSize(fp);
    writeDefaultTileType(fp);
    writeDeviations(fp);
    writeEntities(fp);
    fclose(fp);
}

/**
 * Update the BoardModel with the level data associated with the BoardModel's currently set level number from the
 * binary-packed, persistently stored level file.
 */
void LevelManager::read() {
    // Reset the LevelManager's internal state
    resetLevelManager();

    // Read the level data and update LevelManager's internal state
    std::string levelFile = getPath(board->getLevelNum(), false);
    FILE* fp = fopen(levelFile.c_str(), "rb");
    readSize(fp);
    readDefaultTileType(fp);
    readInitialPlayerCoordinate1to2(fp);
    readDeviations(fp);
    readItems1to2(fp);
    fclose(fp);

    // Populate the BoardModel from LevelManager's internal state
    populateBoard();
}

/**
 * Read the level's items from the persistent storage.
 * @param fp
 */
void LevelManager::readItems1to2(FILE* fp) {
    // Read Count of Items.
    // Hardcoded to 1 since there can only be a key in the puzzle for now.
    uint16_t itemCount;
    fread(&itemCount, sizeof(uint16_t), 1, fp);

    // Read the Key Location.
    // TODO: Add support for more items.
    uint8_t itemIndex;
    uint16_t x;
    uint16_t y;
    fread(&x, sizeof(uint16_t), 1, fp);
    fread(&y, sizeof(uint16_t), 1, fp);
    fread(&itemIndex, sizeof(uint8_t), 1, fp);

    keyCoordinate.setX(static_cast<uint32_t>(x * 25));
    keyCoordinate.setY(static_cast<uint32_t>(y * 25));

    BoardEntity* key = new BaseBoardEntity(keyCoordinate, KEY);
    key->getHitBoxes().push_back(new RectangleHitBox(keyCoordinate, 25, 25));
    entities.push_back(key);
}

/**
 * Read the level's items from the persistent storage.
 * @param fp
 */
void LevelManager::readEntities(FILE* fp) {
    uint16_t count;
    fread(&count, sizeof(uint16_t), 1, fp);

    for (int counter = 0; counter < count; counter++) {

        uint16_t type;
        fread(&type, sizeof(uint16_t), 1, fp);

        long x;
        fread(&x, sizeof(x), 1, fp);

        uint16_t y;
        fread(&y, sizeof(uint32_t), 1, fp);

        BoardEntity* entity = new BaseBoardEntity(Coordinate(x, y), static_cast<BoardEntityType>(type));

        if (type == KEY) {
            keyCoordinate = entity->getCoordinate();
        } else if (type == PLAYER) {
            playerCoordinate = entity->getCoordinate();
        }

    }

}

/**
 * Read the level's deviations to the default tile type from the persistent storage.
 * @param fp
 */
void LevelManager::readDeviations(FILE* fp) {
    // Read Tile Deviation Count
    uint16_t deviationCount;
    fread(&deviationCount, sizeof(uint16_t), 1, fp);

    // Read Each Default Tile Type Deviation & populate each matching tile slot
    // with the deviation.
    for (int currentDeviation = 0; currentDeviation < deviationCount; currentDeviation++) {
        // Read Deviation Tile Coordinate
        uint16_t tileX;
        uint16_t tileY;
        fread(&tileX, sizeof(uint16_t), 1, fp);
        fread(&tileY, sizeof(uint16_t), 1, fp);

        // Read Deviation Tile Type
        uint8_t tileTypeInteger;
        fread(&tileTypeInteger, sizeof(uint8_t), 1, fp);
        auto tileType = TileType(tileTypeInteger);
        deviations[TileCoordinate(tileX, tileY)] = tileType;
    }
}

/**
 * Read the initial tile coordinate of the player.
 * @param fp
 */
void LevelManager::readInitialPlayerCoordinate1to2(FILE* fp) {
    uint16_t x = 0;
    uint16_t y = 0;
    fread(&x, sizeof(uint16_t), 1, fp);
    fread(&y, sizeof(uint16_t), 1, fp);
    playerCoordinate.setX(static_cast<uint32_t>(x * 25));
    playerCoordinate.setY(static_cast<uint32_t>(y * 25));
    BoardEntity* player = new BaseBoardEntity(playerCoordinate, PLAYER);
    player->getHitBoxes().push_back(new RectangleHitBox(playerCoordinate, 25, 25));
    entities.push_back(player);
}

/**
 * Read the default tile type.
 * @param fp
 */
void LevelManager::readDefaultTileType(FILE* fp) {
    // Read Default Tile Type
    uint8_t defaultTileTypeInteger;
    fread(&defaultTileTypeInteger, sizeof(uint8_t), 1, fp);
    defaultTileType = TileType(defaultTileTypeInteger);
}

/**
 * Read the size of the board.
 * <p>
 * This data is currently discarded because the BoardModel height and width are hardcoded. Due to 6/2017 refactoring,
 * this can now be changed.
 * @param fp
 */
void LevelManager::readSize(FILE* fp) {
    // Read Width and Height and discard the result.
    fread(&width, sizeof(uint16_t), 1, fp);
    fread(&height, sizeof(uint16_t), 1, fp);
}

/**
 * Create a new level whose number is determined by the BoardModel.
 */
void LevelManager::create() {
    resetLevelManager();
    populateBoard();
}

/**
 * Build a path to the level file. If inCurrentWorkingDirectory, then assume the path is in the current working
 * directory.
 * @param levelNumber
 * @param inCurrentWorkingDirectory
 */
std::string LevelManager::getPath(uint8_t levelNumber, bool inCurrentWorkingDirectory) const {
    // Determine the prefix of the path.
    std::string prefix = LEVEL_PATH;
    if (inCurrentWorkingDirectory) {
        prefix = "./level/";
    }

    // Build path to Level File.
    std::stringstream ss;
    ss << prefix << (int) levelNumber;
    return ss.str();
}

/**
 * Reset the internal state of the level manager back to defaults.
 */
void LevelManager::resetLevelManager() {
    width = board->getWidth();
    height = board->getHeight();
    playerCoordinate.setX(0);
    playerCoordinate.setY(0);
    keyCoordinate.setX(0);
    keyCoordinate.setY(0);
    defaultTileType = TileType::Empty;
    deviations.clear();
}

/**
 * Update the BoardModel with the data in the internal state of the LevelManager.
 */
void LevelManager::populateBoard() {
    // Populate the remaining tiles with the default tile.  Also, not which
    // tiles have the key and the player.
    uint16_t currentDeviationIndex = 0;
    for (uint16_t tileX = 0; tileX < width; tileX++) {
        for (uint16_t tileY = 0; tileY < height; tileY++) {
            TileCoordinate currentTileCoordinate(tileX, tileY);

            // If the tile is a deviation, use the deviation tile type
            if (deviations.find(currentTileCoordinate) != deviations.end()) {
                board->changeTileType(currentTileCoordinate, deviations[currentTileCoordinate]);
                currentDeviationIndex++;

            // Otherwise, use the default tile type
            } else {
                board->changeTileType(currentTileCoordinate, defaultTileType);
            }
        }
    }

    board->setBoardEntities(entities);
}

/**
 * Write the size of the BoardModel to persistent storage.
 * @param fp
 */
void LevelManager::writeSize(FILE* fp) const {
    // Write Width and Height.
    auto width = static_cast<uint16_t>(board->getWidth());
    auto height = static_cast<uint16_t>(board->getHeight());
    fwrite(&width, sizeof(uint16_t), 1, fp);
    fwrite(&height, sizeof(uint16_t), 1, fp);
}

/**
 * Write the default tile type to persistent storage.
 * @param fp
 */
void LevelManager::writeDefaultTileType(FILE* fp) const {
    // Write Default Tile Type
    auto defaultTileType = static_cast<uint8_t>(TileType::Empty);
    fwrite(&defaultTileType, sizeof(uint8_t), 1, fp);
}

/**
 * Write the tiles that deviate from the default tile type to persistent storage.
 * @param fp
 */
void LevelManager::writeDeviations(FILE* fp) const {
    // Count and Collect the number of Tile Deviations
    std::vector<TileCoordinate> deviatedTileCoordinates;
    for (uint16_t tileX = 0; tileX < board->getWidth(); tileX++) {
        for (uint16_t tileY = 0; tileY < board->getHeight(); tileY++) {
            TileCoordinate tileCoordinate(tileX, tileY);
            if (board->getTileType(tileCoordinate) != defaultTileType) {
                deviatedTileCoordinates.push_back(tileCoordinate);
            }
        }
    }

    // Write Tile Deviation Count
    auto deviationsCount = static_cast<uint16_t>(deviatedTileCoordinates.size());
    fwrite(&deviationsCount, sizeof(uint16_t), 1, fp);

    // Write Each Default Tile Type Deviation.
    for (TileCoordinate tileCoordinate : deviatedTileCoordinates) {
        // Write Deviation Tile Coordinate
        auto x = tileCoordinate.getX();
        auto y = tileCoordinate.getY();
        fwrite(&x, sizeof(x), 1, fp);
        fwrite(&y, sizeof(y), 1, fp);

        // Write Deviation Tile Type
        auto tileType = static_cast<uint8_t>(board->getTileType(tileCoordinate));
        fwrite(&tileType, sizeof(tileType), 1, fp);
    }
}

/**
 * Write each of the board entities into persistent storage.
 * @param fp
 */
void LevelManager::writeEntities(FILE* fp) const {
    // Write the count of Board Entities
    uint16_t count = static_cast<uint16_t>(board->getBoardEntities().size());
    fwrite(&count, sizeof(count), 1, fp);

    // Write each entity
    for (BoardEntity* entity : board->getBoardEntities()) {

        uint16_t type = entity->getType();
        fwrite(&type, sizeof(type), 1, fp);

        Coordinate coordinate = entity->getCoordinate();

        long x = coordinate.getX();
        fwrite(&x, sizeof(x), 1, fp);

        long y = coordinate.getY();
        fwrite(&y, sizeof(y), 1, fp);
    }

}
