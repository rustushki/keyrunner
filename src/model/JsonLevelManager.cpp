#include <fstream>
#include "../controller/KeyRunner.hpp"
#include "../model/BaseBoardEntity.hpp"
#include "../model/JsonLevelManager.hpp"
#include "../model/RectangleHitBox.hpp"
#include "../model/TileCoordinate.hpp"

/**
 * Get the total count of levels available for loading.
 * @return uint8_t
 */
uint8_t JsonLevelManager::getLevelCount() const {
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
bool JsonLevelManager::levelExists(uint8_t levelNumber) const {
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
 * Read the level into the BoardModel.
 * <p>
 * The level number is set within the BoardModel.
 * @param boardModel
 */
void JsonLevelManager::read(BoardModel* boardModel) {
    // Determine the path to the level file that must be read
    std::string levelFile = getPath(boardModel->getLevelNum(), true);

    // Open the file
    std::ifstream fileStream(levelFile, std::ifstream::in | std::ifstream::binary);

    // Get the length of the file
    fileStream.seekg(0, fileStream.end);
    long length = fileStream.tellg();
    fileStream.seekg(0, fileStream.beg);

    // Read every byte of the file into a buffer
    auto buffer = new char[length];
    fileStream.read(buffer, length);
    fileStream.close();

    // Convert the buffer into a json object
    json j = json::parse(std::string(buffer));

    // Free the buffer
    delete[] buffer;

    // Convert it into a board model
    fromJson(j, boardModel);
}

/**
 * Write a level file out based on the provided BoardModel.
 * @param boardModel
 */
void JsonLevelManager::write(const BoardModel* boardModel) const {
    std::string levelFile = getPath(boardModel->getLevelNum(), true);

    json levelJson;
    toJson(levelJson, boardModel);

    FILE* fp = fopen(levelFile.c_str(), "wb");
    std::string levelJsonString = levelJson.dump(2);
    fwrite(levelJsonString.c_str(), levelJsonString.size(), 1, fp);
    fclose(fp);
}

/**
 * Initialize a BoardModel as an empty level.
 * @param boardModel
 */
void JsonLevelManager::create(BoardModel* boardModel) {

}

/**
 * Create a path string to the level file associated with the level.
 * <p>
 * If the flag is set, assume that the path prefix should be in the current working directory.
 * @param levelNumber
 * @param inCurrentWorkingDirectory
 * @return path to level
 */
std::string JsonLevelManager::getPath(uint8_t levelNumber, bool inCurrentWorkingDirectory) const {
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
 * Populate the provided json object from the provided BoardModel.
 * @param j
 * @param boardModel
 */
void JsonLevelManager::toJson(json& j, const BoardModel* boardModel) const {
    TileType defaultTileType = TileType::Empty;

    // Count and Collect the number of Tile Deviations
    std::vector<TileCoordinate> deviatedTileCoordinates;
    for (uint16_t tileX = 0; tileX < boardModel->getWidth(); tileX++) {
        for (uint16_t tileY = 0; tileY < boardModel->getHeight(); tileY++) {
            TileCoordinate tileCoordinate(tileX, tileY);
            if (boardModel->getTileType(tileCoordinate) != defaultTileType) {
                deviatedTileCoordinates.push_back(tileCoordinate);
            }
        }
    }

    json jDeviationList;
    for (TileCoordinate deviatedTileCoordinate : deviatedTileCoordinates) {
        json jDeviation;
        toJson(jDeviation["tileCoordinate"], &deviatedTileCoordinate);
        jDeviation["deviationTileType"] = boardModel->getTileType(deviatedTileCoordinate);
        jDeviationList += jDeviation;
    }

    json jEntityList;
    jEntityList += {
            {"x", boardModel->getKeyCoord().getX()},
            {"y", boardModel->getKeyCoord().getY()},
            {"type", BoardEntityType::KEY}
    };

    jEntityList += {
            {"x", boardModel->getPlayerCoord().getX()},
            {"y", boardModel->getPlayerCoord().getY()},
            {"type", BoardEntityType::PLAYER}
    };

    json board;
    board["version"] = 1;
    board["width"] = boardModel->getWidth();
    board["height"] = boardModel->getHeight();
    board["levelNumber"] = boardModel->getLevelNum();
    board["defaultTileType"] = defaultTileType;
    board["deviations"] = jDeviationList;
    board["entities"] = jEntityList;

    j["board"] = board;
}

/**
 * Populate the provided BoardModel from the provided json object.
 * @param j
 * @param boardModel
 */
void JsonLevelManager::fromJson(const json& j, BoardModel* boardModel) const {
    json board = j.at("board");

    int width = board.at("width");
    int height = board.at("height");
    boardModel->setLevelNum(board.at("levelNumber"));

    TileType defaultTileType = board.at("defaultTileType");

    std::map<TileCoordinate, TileType> deviatedTiles;
    json deviations = board.at("deviations");
    for (json deviation : deviations) {
        TileCoordinate tileCoordinate;
        fromJson(deviation.at("tileCoordinate"), &tileCoordinate);
        TileType deviationTileType = deviation.at("deviationTileType");
        deviatedTiles[tileCoordinate] = deviationTileType;
    }

    // Remove all existing wall hit boxes
    boardModel->getWallHitBoxes().clear();

    // Populate the remaining tiles with the default tile. Also, not which tiles have the key and the player. Create
    // wall hit boxes as wall tiles are encountered
    uint16_t currentDeviationIndex = 0;
    for (uint16_t tileX = 0; tileX < width; tileX++) {
        for (uint16_t tileY = 0; tileY < height; tileY++) {
            TileCoordinate currentTileCoordinate(tileX, tileY);

            // If the tile is a deviation, use the deviation tile type
            if (deviatedTiles.find(currentTileCoordinate) != deviatedTiles.end()) {
                TileType tileType = deviatedTiles[currentTileCoordinate];

                // If the tile is a wall, build a wall hit box for it
                if (tileType == TileType::Wall) {
                    const int size = TileCoordinate::SIZE;
                    Coordinate anchor = currentTileCoordinate.toCoordinate();
                    HitBox* wallHitBox = new RectangleHitBox(anchor, size, size);
                    boardModel->getWallHitBoxes().push_back(wallHitBox);
                }

                boardModel->changeTileType(currentTileCoordinate, tileType);
                currentDeviationIndex++;

                // Otherwise, use the default tile type
            } else {
                boardModel->changeTileType(currentTileCoordinate, defaultTileType);
            }
        }
    }

    std::vector<BoardEntity*> entityList;
    json entities = board.at("entities");
    for (json entity : entities) {
        long x = entity.at("x");
        long y = entity.at("y");
        BoardEntityType type = entity.at("type");
        entityList.push_back(new BaseBoardEntity(Coordinate(x, y), type));
    }

    boardModel->setBoardEntities(entityList);
}

/**
 * Populate the provided json object from the provided TileCoordinate.
 * @param j
 * @param tileCoordinate
 */
void JsonLevelManager::toJson(json& j, const TileCoordinate* tileCoordinate) const {
    j["tileX"] = tileCoordinate->getX();
    j["tileY"] = tileCoordinate->getY();
}

/**
 * Populate the provided TileCoordinate from the provided json object.
 * @param j
 * @param tileCoordinate
 */
void JsonLevelManager::fromJson(const json& j, TileCoordinate* tileCoordinate) const {
    tileCoordinate->setX(j.at("tileX"));
    tileCoordinate->setY(j.at("tileY"));
}
