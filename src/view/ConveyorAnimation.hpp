#ifndef CONVEYOR_HPP
#define CONVEYOR_HPP

#include <stdint.h>
#include <vector>

#include "../model/PlayModel.hpp"

class TileLayer;

class ConveyorAnimation {
public:
    ConveyorAnimation(std::vector<TileCoord>);
    ~ConveyorAnimation();

    static bool TileInConveyor(TileCoord tileCoord);
    static bool StartConveyors();
    static void ClearConveyors();
    static void BuildConveyorAnimations();

    bool startAnimation();

private:

    std::vector<TileCoord> conveyorTiles;
    bool hasTile(TileCoord tileCoord) const;

    uint16_t nextTileToStart;

    static std::vector<ConveyorAnimation*> Conveyors;
};

#endif//CONVEYOR_HPP
