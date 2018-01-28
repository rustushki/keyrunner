#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include <cstdint>
#include "../model/BoardModel.hpp"

class LevelManager {
public:
    virtual uint8_t getLevelCount() const = 0;
    virtual bool levelExists(uint8_t levelNumber) const = 0;
    virtual void read(BoardModel* boardModel) = 0;
    virtual void write(const BoardModel* boardModel) const = 0;
    virtual void create(BoardModel* boardModel) = 0;
};

#endif
