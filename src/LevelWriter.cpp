#include <sstream>
#include <vector>

#include "KeyRunner.hpp"
#include "Level.hpp"
#include "LevelWriter.hpp"
#include "TileType.hpp"

bool LevelWriter::Write(Level* level) {
    // Build path to Level File.
    std::stringstream ss;
    ss << "./" << level->toInt() << ".bin";
    std::string levelFile = ss.str();

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
            devTiles.push_back(level->getTile(x, y));
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
