#ifndef PLAY_BOARD_HPP
#define PLAY_BOARD_HPP

#include "../model/BoardModel.hpp"
#include "../model/Direction.hpp"
#include "../model/Model.hpp"
#include "../model/State.hpp"

class PlayBoardModel : public BoardModel {
public:
    PlayBoardModel();

    bool isComplete() const;
    void movePlayerInDirection(Direction direction);

    long getTimeClock();
    void setTimeClock(long timeClockValue);
    void decrementTimeClock(long step);
    void incrementTimeClock(long step);

    void conveyPlayer();

private:
    long timeClock;

    std::set<TileCoordinate> getTilesHoldingPlayer() const;
};

#endif
