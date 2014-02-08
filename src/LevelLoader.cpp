#include <sstream>

#include "KeyRunner.hpp"
#include "LevelLoader.hpp"

uint16_t LevelLoader::GetTotal() {

    static int levelCount = -1;

    if (levelCount == -1) {
        int hiLevel = 10000;
        int loLevel =     1;
        int miLevel =     0;

        while (hiLevel >= loLevel) {

            miLevel = (hiLevel-loLevel)/2 + loLevel;

            std::stringstream ss;
            ss << LEVELPATH << miLevel;
            FILE* fp = fopen(ss.str().c_str(), "r");

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

