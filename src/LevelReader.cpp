#include <sstream>

#include "Level.hpp"
#include "LevelReader.hpp"
#include "TileType.hpp"

Level* LevelReader::Read(uint8_t levelNum) {
    // Build path to Level File.
    std::string levelFile = LEVELPATH;
    std::stringstream ss;
    ss << (int)levelNum;
    levelFile += ss.str();


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
