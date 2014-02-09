#include <sstream>
#include <vector>

#include "KeyRunner.hpp"
#include "Level.hpp"
#include "LevelManager.hpp"
#include "TileType.hpp"

uint16_t LevelManager::GetTotal() {

    static int levelCount = -1;

    if (levelCount == -1) {
        uint8_t hiLevel = 255;
        uint8_t loLevel =   1;
        uint8_t miLevel =   0;

        while (hiLevel >= loLevel) {

            miLevel = (hiLevel-loLevel)/2 + loLevel;

            std::string levelFile = GetPath(miLevel, false);
            FILE* fp = fopen(levelFile.c_str(), "rb");

            if (fp != NULL) {
                fclose(fp);
                loLevel = miLevel + 1;
            } else {
                hiLevel = miLevel - 1;
            }
        }
        levelCount = miLevel;
    }

    return levelCount;

}

bool LevelManager::Write(Level* level) {
    // Open the Level File.
    std::string levelFile = GetPath(level->toInt(), true);
    FILE* fp = fopen(levelFile.c_str(), "wb");

    // Write Width and Height.
    uint16_t w = static_cast<uint16_t>(GridLayer::GRID_WIDTH);
    uint16_t h = static_cast<uint16_t>(GridLayer::GRID_HEIGHT);
    fwrite(&w, sizeof(uint16_t), 1, fp);
    fwrite(&h, sizeof(uint16_t), 1, fp);

    // Write Default Tile Type
    uint8_t tt = static_cast<uint8_t>(TILETYPE_EMPTY);
    fwrite(&tt, sizeof(uint8_t), 1, fp);

    // Count and Collect the number of Tile Deviations
    std::vector<Tile*> devTiles;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            Tile* tile = level->getTile(x, y);
            if (tile->getType() != tt) {
                devTiles.push_back(tile);
            }
        }
    }

    // Write Tile Deviation Count
    uint16_t devCount = devTiles.size();
    fwrite(&devCount, sizeof(uint16_t), 1, fp);

    // Write Initial Char Location.
    Tile* pTile = level->getPlayerTile();
    uint16_t x = pTile->getX();
    uint16_t y = pTile->getY();
    fwrite(&x, sizeof(uint16_t), 1, fp);
    fwrite(&y, sizeof(uint16_t), 1, fp);

    // Write Count of Items.
    // Hardcoded to 1 since there can only be a key in the puzzle for now.
    uint16_t itemCount = 1;
    fwrite(&itemCount, sizeof(uint16_t), 1, fp);

    // Write Each Default Tile Type Deviation.
    for (auto dtItr = devTiles.begin(); dtItr < devTiles.end(); dtItr++) {
        Tile* devTile = *dtItr;

        // Write Deviation Tile Coordinate
        uint16_t x = devTile->getX();
        uint16_t y = devTile->getY();
        fwrite(&x, sizeof(uint16_t), 1, fp);
        fwrite(&y, sizeof(uint16_t), 1, fp);

        // Write Deviation Tile Type
        uint8_t tt = static_cast<uint8_t>(devTile->getType());
        fwrite(&tt, sizeof(uint8_t), 1, fp);
    }

    // Write Each Item Type
    // For now, we only need to record the location of the key.
    Tile* kTile = level->getKeyTile();
    x = kTile->getX();
    y = kTile->getY();
    uint8_t it = 0;
    fwrite(&x, sizeof(uint16_t), 1, fp);
    fwrite(&y, sizeof(uint16_t), 1, fp);
    fwrite(&it, sizeof(uint8_t), 1, fp);

    fclose(fp);
    return true;
}

Level* LevelManager::Read(uint8_t levelNum) {
    // Open the Level File.
    std::string levelFile = GetPath(levelNum, false);
    FILE* fp = fopen(levelFile.c_str(), "rb");

    // Read Width and Height and discard the result.
    // TODO: Currently the height and width are hard coded as GRID_HEIGHT and
    // GRID_WIDTH.
    uint16_t w = 0;
    uint16_t h = 0;
    fread(&w, sizeof(uint16_t), 1, fp);
    fread(&h, sizeof(uint16_t), 1, fp);

    // Read Default Tile Type
    uint8_t defTTint;
    fread(&defTTint, sizeof(uint8_t), 1, fp);
    TileType defTT = (TileType) defTTint;

    // Read Tile Deviation Count
    uint16_t devCount;
    fread(&devCount, sizeof(uint16_t), 1, fp);

    // Read Initial Char Location.
    uint16_t px;
    uint16_t py;
    fread(&px, sizeof(uint16_t), 1, fp);
    fread(&py, sizeof(uint16_t), 1, fp);

    // Read Count of Items.
    // Hardcoded to 1 since there can only be a key in the puzzle for now.
    uint16_t itemCount;
    fread(&itemCount, sizeof(uint16_t), 1, fp);

    // Build Level and initialize all Tiles to NULL.
    Level* level = new Level(levelNum);
    for (int ty = h - 1; ty >= 0; ty--) {
        for (int tx = w - 1; tx >= 0; tx--) {
            level->tile[ty][tx] = NULL;
        }
    }

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

        Tile* tile = new Tile(tt, x, y, level);
        level->tile[tile->getY()][tile->getX()] = tile;
    }

    // Read the Key Location.
    // TODO: Add support for more items.
    uint16_t kx;
    uint16_t ky;
    uint8_t it;
    fread(&kx, sizeof(uint16_t), 1, fp);
    fread(&ky, sizeof(uint16_t), 1, fp);
    fread(&it, sizeof(uint8_t), 1, fp);

    // Populate the remaining tiles with the default tile.  Also, not which
    // tiles have the key and the player.
    for (int ty = h - 1; ty >= 0; ty--) {
        for (int tx = w - 1; tx >= 0; tx--) {
            if (level->tile[ty][tx] == NULL) {
                // Build a Default Tile at the coordinate.
                Tile* tile = new Tile(defTT, tx, ty, level);

                // Add the tile to the level.
                level->tile[ty][tx] = tile;
            }

            // TODO: support multiple keys.

            // Does the current tile have the player?
            if (tx == px && ty == py) {
                level->tileHasPlayer = level->tile[ty][tx];
            }

            // Does the current tile have the key?
            if (tx == kx && ty == ky) {
                level->tileHasKey = level->tile[ty][tx];
            }

        }
    }
    level->init();

    fclose(fp);

    return level;

}

Level* LevelManager::New(uint8_t levelNum) {
    std::cout << "new level stub called" << std::endl;
    Level* level = new Level(levelNum);
    level->init();
    return level;
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
