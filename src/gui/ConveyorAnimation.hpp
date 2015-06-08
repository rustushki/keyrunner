#ifndef CONVEYOR_HPP
#define CONVEYOR_HPP

#include <stdint.h>
#include <vector>

class TileLayer;

class ConveyorAnimation {
public:
    ConveyorAnimation(std::vector<TileLayer*>);
    ~ConveyorAnimation();

    static bool TileInConveyor(TileLayer* tile);
    static bool StartConveyors();
    static void ClearConveyors();

    bool startAnimation();

private:

    std::vector<TileLayer*> conveyorTiles;
    bool hasTile(TileLayer*) const;

    uint16_t nextTileToStart;

    static std::vector<ConveyorAnimation*> Conveyors;
};

#endif//CONVEYOR_HPP
