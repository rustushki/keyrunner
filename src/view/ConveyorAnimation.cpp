#include <iostream>
#include "ConveyorAnimation.hpp"
#include "GridLayer.hpp"
#include "TileLayer.hpp"
#include "../controller/KeyRunner.hpp"

std::vector<ConveyorAnimation*> ConveyorAnimation::Conveyors;

ConveyorAnimation::ConveyorAnimation(std::vector<TileCoord> conveyorTiles) {
    this->conveyorTiles = conveyorTiles;

    Conveyors.push_back(this);

    this->nextTileToStart = 0;
}

ConveyorAnimation::~ConveyorAnimation(){ }

/* ------------------------------------------------------------------------------
 * startAnimation - Attempts to start the next tile in the chained conveyor
 * belt animation.  Returns true if all tiles have been started.  For best
 * results, call this method each time a new screen frame is blitted.
 */
bool ConveyorAnimation::startAnimation() {
    if (this->nextTileToStart >= this->conveyorTiles.size()) {
        return true;
    }

    GridLayer* gl = GridLayer::GetInstance();

    TileLayer* currentTile = NULL;
    if (this->nextTileToStart == 0) {

        // Always start the first tile in the belt.
        TileCoord currentTileCoord = this->conveyorTiles[0];
        currentTile = gl->getTile(currentTileCoord.first, currentTileCoord.second);
        currentTile->getAnimation()->play();
        gl->pushAnimatedTile(currentTile);
        this->nextTileToStart++;

    } else {
        TileCoord currentTileCoord = this->conveyorTiles[this->nextTileToStart-1];
        currentTile = gl->getTile(currentTileCoord.first, currentTileCoord.second);

        if ((currentTile->getAnimation()->getCurrentStill()+1) % 8 == 0) {
            TileCoord tileCoord = this->conveyorTiles[this->nextTileToStart];
            TileLayer* tile = gl->getTile(tileCoord.first, tileCoord.second);
            tile->getAnimation()->play();
            gl->pushAnimatedTile(tile);

            this->nextTileToStart++;
        }
    }

    return (this->nextTileToStart >= this->conveyorTiles.size());

}

bool ConveyorAnimation::hasTile(TileCoord tileCoord) const {
    GridLayer* gl = GridLayer::GetInstance();
    for (uint16_t x = 0; x < this->conveyorTiles.size(); x++) {
        TileCoord conveyorTileCoord = this->conveyorTiles[x];
        TileLayer* conveyorTile = gl->getTile(conveyorTileCoord.first, conveyorTileCoord.second);
        if (gl->getTile(tileCoord.first, tileCoord.second) == conveyorTile) {
            return true;
        }
    }

    return false;
}

/* ------------------------------------------------------------------------------
 * TileInConveyor - Check all conveyors to see if the provided tile is already
 * in a conveyor belt.
 */
bool ConveyorAnimation::TileInConveyor(TileCoord tileCoord) {

    for (uint16_t c = 0; c < Conveyors.size(); c++) {
        ConveyorAnimation* conveyor = Conveyors[c];
        if (conveyor->hasTile(tileCoord)) {

            return true;

        }
    }

    return false;
}

/* ------------------------------------------------------------------------------
 * StartConveyors - For each ConveyorAnimation currently loaded, attempt to
 * start the next tile in each.  This method ought to be called once per frame
 * blit until it returns true.
 */
bool ConveyorAnimation::StartConveyors() {
    bool allStarted = true;
    for (uint16_t x = 0; x < Conveyors.size(); x++) {
        ConveyorAnimation* ca = Conveyors[x];
        bool started = ca->startAnimation();

        allStarted &= started;
    }

    return allStarted;
}

void ConveyorAnimation::ClearConveyors() {
    Conveyors.clear();
}

void ConveyorAnimation::BuildConveyorAnimations() {

    ClearConveyors();

    PlayModel* playModel = PlayModel::GetInstance();

    for (int x = 0; x < PlayModel::GRID_WIDTH; x++) {

        for (int y = 0; y < PlayModel::GRID_HEIGHT; y++) {
            TileCoord tileCoord = TileCoord(x, y);

            if (playModel->isConveyor(tileCoord)) {

                if (!TileInConveyor(tileCoord)) {

                    // This TileCoord must be part of a Conveyor.  Find the
                    // start of the Conveyor.

                    TileCoord q = tileCoord;
                    TileCoord p = q;

                    if (TileInConveyor(tileCoord)) {
                        continue;
                    }

                    Direction conveyDir = playModel->getConveyorDirection(q);

                    // Initialize oppDir to something invalid to pacify
                    // compiler warnings.
                    Direction oppDir = DIRECTION_COUNT;

                    // Determine the opposite direction of this belt.
                    if (conveyDir == DIRECTION_UP) {
                        oppDir = DIRECTION_DOWN;
                    } else if (conveyDir == DIRECTION_DOWN) {
                        oppDir = DIRECTION_UP;
                    } else if (conveyDir == DIRECTION_LEFT) {
                        oppDir = DIRECTION_RIGHT;
                    } else if (conveyDir == DIRECTION_RIGHT) {
                        oppDir = DIRECTION_LEFT;
                    }

                    // A circular conveyor is one which stretches from one
                    // border to the other, causing it it wrap-around.  We need
                    // to check for this behavior so that we can prevent
                    // infinite looping here.
                    bool circular = false;

                    // Go backwards along the belt until you hit a non-conveyor
                    // tile, or a conveyor tile which is part of another
                    // conveyor, or we find that the current belt is circular.
                    TileCoord prev = p;
                    while (    playModel->isConveyor(p)
                            && playModel->getConveyorDirection(p) == conveyDir
                            && !TileInConveyor(p)
                            && !circular) {

                        prev = p;

                        if (oppDir == DIRECTION_UP) {
                            p = playModel->getTileCoordUp(p);
                        } else if (oppDir == DIRECTION_DOWN) {
                            p = playModel->getTileCoordDown(p);
                        } else if (oppDir == DIRECTION_RIGHT) {
                            p = playModel->getTileCoordRight(p);
                        } else if (oppDir == DIRECTION_LEFT) {
                            p = playModel->getTileCoordLeft(p);
                        }

                        if (p == q) {
                            circular = true;
                        }

                    }

                    TileCoord start = p = prev;
                    std::vector<TileCoord> conveyorTiles;

                    int tileNum = 0;
                    // Now follow the conveyor from its start until a
                    // non-conveyor tile, or a conveyor tile which is part of
                    // another conveyor or if the belt is found to be circular,
                    // the start tile.
                    while (    playModel->isConveyor(p)
                            && playModel->getConveyorDirection(p) == conveyDir
                            && !TileInConveyor(p)) {

                        conveyorTiles.push_back(p);

                        tileNum++;

                        if (conveyDir == DIRECTION_UP) {
                            p = playModel->getTileCoordUp(p);
                        } else if (conveyDir == DIRECTION_DOWN) {
                            p = playModel->getTileCoordDown(p);
                        } else if (conveyDir == DIRECTION_RIGHT) {
                            p = playModel->getTileCoordRight(p);
                        } else if (conveyDir == DIRECTION_LEFT) {
                            p = playModel->getTileCoordLeft(p);
                        }

                        if (circular) {
                            if (p == start) {
                                break;
                            }
                        }
                    }

                    // Creating a ConveyorAnimation causes it to be added to
                    // the ConveyorAnimation array.
                    new ConveyorAnimation(conveyorTiles);
                }
            }
        }
    }
}


