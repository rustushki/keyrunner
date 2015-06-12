#include <sstream>

#include "LevelManager.hpp"
#include "../controller/KeyRunner.hpp"
#include "../model/PlayModel.hpp"
#include "../view/GridLayer.hpp"

uint16_t LevelManager::w;
uint16_t LevelManager::h;
TileType LevelManager::defTT;
std::map<TileCoord, TileType> LevelManager::deviations;
TileCoord LevelManager::playerCoord;
TileCoord LevelManager::keyCoord;

uint16_t LevelManager::GetTotal() {

    static int levelCount = -1;

    if (levelCount == -1) {
        uint8_t hiLevel = 255;
        uint8_t loLevel =   0;
        uint8_t miLevel =   0;

        while (hiLevel >= loLevel) {
            miLevel = (hiLevel-loLevel)/2 + loLevel;

            if (LevelManager::Exists(miLevel) && !LevelManager::Exists(miLevel + 1)) {
                levelCount = miLevel;
                break;
            } else if (LevelManager::Exists(miLevel)) {
                loLevel = miLevel + 1;
            } else {
                hiLevel = miLevel - 1;
            }
        }
        levelCount = miLevel;
    }

    return levelCount;

}

bool LevelManager::Exists(uint8_t levelNum) {
    std::string levelFile = GetPath(levelNum, false);
    FILE* fp = fopen(levelFile.c_str(), "rb");

    bool exists = false;
    if (fp != NULL) {
        exists = true;
        fclose(fp);
    }

    return exists;
}

/* ------------------------------------------------------------------------------
 * Write - Writes the current state of the PlayModel into a static level file.
 */
bool LevelManager::Write() {
    PlayModel* playModel = PlayModel::GetInstance();

    // Open the Level File.
    std::string levelFile = GetPath(playModel->getLevelNum(), true);
    FILE* fp = fopen(levelFile.c_str(), "wb");

    // Write Width and Height.
    uint16_t w = static_cast<uint16_t>(PlayModel::GRID_WIDTH);
    uint16_t h = static_cast<uint16_t>(PlayModel::GRID_HEIGHT);
    fwrite(&w, sizeof(uint16_t), 1, fp);
    fwrite(&h, sizeof(uint16_t), 1, fp);

    // Write Default Tile Type
    uint8_t tt = static_cast<uint8_t>(TILETYPE_EMPTY);
    fwrite(&tt, sizeof(uint8_t), 1, fp);

    // Count and Collect the number of Tile Deviations
    std::vector<TileCoord> devTileCoords;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            TileCoord tileCoord(x, y);
            if (playModel->getTileType(tileCoord) != tt) {
                devTileCoords.push_back(tileCoord);
            }
        }
    }

    // Write Tile Deviation Count
    uint16_t devCount = devTileCoords.size();
    fwrite(&devCount, sizeof(uint16_t), 1, fp);

    // Write Initial Char Location.
    TileCoord playerCoord = playModel->getPlayerCoord();
    uint16_t x = playerCoord.first;
    uint16_t y = playerCoord.second;
    fwrite(&x, sizeof(uint16_t), 1, fp);
    fwrite(&y, sizeof(uint16_t), 1, fp);

    // Write Count of Items.
    // Hardcoded to 1 since there can only be a key in the puzzle for now.
    uint16_t itemCount = 1;
    fwrite(&itemCount, sizeof(uint16_t), 1, fp);

    // Write Each Default Tile Type Deviation.
    for (auto dtItr = devTileCoords.begin(); dtItr < devTileCoords.end(); dtItr++) {
        TileCoord tileCoord = *dtItr;

        // Write Deviation Tile Coordinate
        uint16_t x = tileCoord.first;
        uint16_t y = tileCoord.second;
        fwrite(&x, sizeof(uint16_t), 1, fp);
        fwrite(&y, sizeof(uint16_t), 1, fp);

        // Write Deviation Tile Type
        uint8_t tt = static_cast<uint8_t>(playModel->getTileType(tileCoord));
        fwrite(&tt, sizeof(uint8_t), 1, fp);
    }

    // Write Each Item Type
    // For now, we only need to record the location of the key.
    TileCoord kCoord = PlayModel::GetInstance()->getKeyCoord();
    x = kCoord.first;
    y = kCoord.second;
    uint8_t it = 0;
    fwrite(&x, sizeof(uint16_t), 1, fp);
    fwrite(&y, sizeof(uint16_t), 1, fp);
    fwrite(&it, sizeof(uint8_t), 1, fp);

    fclose(fp);
    return true;
}

void LevelManager::Read(uint8_t levelNum) {
    Reset();

    // Open the Level File.
    std::string levelFile = GetPath(levelNum, false);
    FILE* fp = fopen(levelFile.c_str(), "rb");

    // Read Width and Height and discard the result.
    // TODO: Currently the height and width are hard coded as GRID_HEIGHT and
    // GRID_WIDTH.
    fread(&w, sizeof(uint16_t), 1, fp);
    fread(&h, sizeof(uint16_t), 1, fp);

    // Read Default Tile Type
    uint8_t defTTint;
    fread(&defTTint, sizeof(uint8_t), 1, fp);
    defTT = (TileType) defTTint;

    // Read Tile Deviation Count
    uint16_t devCount;
    fread(&devCount, sizeof(uint16_t), 1, fp);

    // Read Initial Char Location.
    fread(&playerCoord.first, sizeof(uint16_t), 1, fp);
    fread(&playerCoord.second, sizeof(uint16_t), 1, fp);

    // Read Count of Items.
    // Hardcoded to 1 since there can only be a key in the puzzle for now.
    uint16_t itemCount;
    fread(&itemCount, sizeof(uint16_t), 1, fp);

    // Read Each Default Tile Type Deviation & populate each matching tile slot
    // with the deviation.
    for (int ttd = 0; ttd < devCount; ttd++) {
        // Write Deviation Tile Coordinate
        uint16_t x;
        uint16_t y;
        fread(&x, sizeof(uint16_t), 1, fp);
        fread(&y, sizeof(uint16_t), 1, fp);

        // Write Deviation Tile Type
        uint8_t ttInt;
        fread(&ttInt, sizeof(uint8_t), 1, fp);
        TileType tt = static_cast<TileType>(ttInt);

        deviations[TileCoord(x, y)] = tt;
    }

    // Read the Key Location.
    // TODO: Add support for more items.
    uint8_t it;
    fread(&keyCoord.first, sizeof(uint16_t), 1, fp);
    fread(&keyCoord.second, sizeof(uint16_t), 1, fp);
    fread(&it, sizeof(uint8_t), 1, fp);
    fclose(fp);

    Populate(levelNum);
}

void LevelManager::New(uint8_t levelNum) {
    Reset();
    Populate(levelNum);
}

/* ------------------------------------------------------------------------------
 * GetPath - build a path to the level file.  if inCwd, then assume the path is
 * in the current working directory.
 */
std::string LevelManager::GetPath(uint8_t levelNum, bool inCwd) {
    std::string prefix = LEVELPATH;
    if (inCwd) {
        prefix = "./";
    }

    // Build path to Level File.
    std::stringstream ss;
    ss << prefix << (int) levelNum;
    return ss.str();
}

void LevelManager::Reset() {
    w = PlayModel::GRID_WIDTH;
    h = PlayModel::GRID_HEIGHT;
    playerCoord.first = 0;
    playerCoord.second = 0;
    keyCoord.first = w-1;
    keyCoord.second = h-1;
    defTT = TileType::TILETYPE_EMPTY;
    deviations.clear();
}

void LevelManager::Populate(uint8_t levelNum) {
    // Get the GridLayer instance.
    GridLayer* gl  = GridLayer::GetInstance();
    PlayModel* playModel  = PlayModel::GetInstance();

    // Set the Level in the GridLayer.
    playModel->setLevelNum(levelNum);

    // Player does not have key at level start.
    playModel->setPlayerHasKey(false);

    // Populate the remaining tiles with the default tile.  Also, not which
    // tiles have the key and the player.
    uint16_t curDevIdx = 0;
    for (int tx = 0; tx < w; tx++) {
        for (int ty = 0; ty < h; ty++) {
            TileCoord curTileCoord(tx, ty);

            bool deviationMatch = false;
            if (deviations.find(curTileCoord) != deviations.end()) {
                gl->changeTileType(tx, ty, deviations[curTileCoord]);
                playModel->changeTileType(curTileCoord, deviations[curTileCoord]);
                curDevIdx++;
                deviationMatch = true;
            }

            if (!deviationMatch) {
                // Add the tile to the level.
                gl->changeTileType(tx, ty, defTT);
                playModel->changeTileType(curTileCoord, defTT);
            }

            // TODO: Support multiple items. (not just hard coded key and
            // player)

            // Does the current tile have the player?
            if (curTileCoord == playerCoord) {
                playModel->setPlayerCoord(curTileCoord);
            }

            // Does the current tile have the key?
            if (curTileCoord == keyCoord) {
                playModel->setKeyCoord(curTileCoord);
            }

        }
    }
    gl->init();
}
