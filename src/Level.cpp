#include <cstdio>

#include "Animation.hpp"
#include "ConveyorAnimation.hpp"
#include "GridLayer.hpp"
#include "KeyRunner.hpp"
#include "Level.hpp"
#include "TileType.hpp"

Level::Level(int levelNum) {
    level = levelNum;
    playerHasKey = false;
    tileHasPlayer = NULL;
    tileHasKey = NULL;

}

Level::~Level() {
    GridLayer::GetInstance()->clearChangedTiles();
    for (int x = 0; x < GridLayer::GRID_WIDTH; x++) {
        for (int y = 0; y < GridLayer::GRID_HEIGHT; y++) {
            delete tile[y][x];
        }
    }
}

void Level::init() {
    GridLayer::GetInstance()->clearAnimatedTiles();

    buildConveyorAnimations();
}

void Level::buildConveyorAnimations() {

    ConveyorAnimation::ClearConveyors();

    for (int x = 0; x < GridLayer::GRID_WIDTH; x++) {

        for (int y = 0; y < GridLayer::GRID_HEIGHT; y++) {
            Tile* tile = this->getTile(x, y);

            if (tile->isConveyor()) {

                if (!ConveyorAnimation::TileInConveyor(tile)) {

                    // This Tile must be part of a Conveyor.  Find the start of
                    // the Conveyor.

                    Tile* q = tile;
                    Tile* p = q;

                    if (ConveyorAnimation::TileInConveyor(p)) {
                        continue;
                    }

                    //std::cout << "Found Tile: " << q->getX() << "," << q->getY() << std::endl;

                    Direction conveyDir = q->getConveyorDirection();

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

                    //std::cout << "\tOppDir: " << oppDir << std::endl;


                    // A circular conveyor is one which stretches from one
                    // border to the other, causing it it wrap-around.  We need
                    // to check for this behavior so that we can prevent
                    // infinite looping here.
                    bool circular = false;

                    // Go backwards along the belt until you hit a non-conveyor
                    // tile, or a conveyor tile which is part of another
                    // conveyor, or we find that the current belt is circular.
                    Tile* prev = p;
                    while (    p->isConveyor()
                            && p->getConveyorDirection() == conveyDir
                            && !ConveyorAnimation::TileInConveyor(p)
                            && !circular) {

                        prev = p;

                        if (oppDir == DIRECTION_UP) {
                            p = p->up();
                        } else if (oppDir == DIRECTION_DOWN) {
                            p = p->down();
                        } else if (oppDir == DIRECTION_RIGHT) {
                            p = p->right();
                        } else if (oppDir == DIRECTION_LEFT) {
                            p = p->left();
                        }

                        if (p == q) {
                            circular = true;
                        }

                    }

                    Tile* start = p = prev;
                    std::vector<Tile*> conveyorTiles;

                    //std::cout << "\tStart " << start->getX() << "," << start->getY() << std::endl;
                    //std::cout << "\t" << p->isConveyor() << std::endl;
                    //std::cout << "\t" << p->getConveyorDirection() << "=" << conveyDir << std::endl;
                    //std::cout << "\t" << ConveyorAnimation::TileInConveyor(p) << std::endl;

                    int tileNum = 0;
                    // Now follow the conveyor from its start until a
                    // non-conveyor tile, or a conveyor tile which is part of
                    // another conveyor or if the belt is found to be circular,
                    // the start tile.
                    while (    p->isConveyor()
                            && p->getConveyorDirection() == conveyDir
                            && !ConveyorAnimation::TileInConveyor(p)) {

                        conveyorTiles.push_back(p);

                        tileNum++;
                        //std::cout << "\ttile #" << tileNum << " " << p->getX() << "," << p->getY() << std::endl;

                        if (conveyDir == DIRECTION_UP) {
                            p = p->up();
                        } else if (conveyDir == DIRECTION_DOWN) {
                            p = p->down();
                        } else if (conveyDir == DIRECTION_RIGHT) {
                            p = p->right();
                        } else if (conveyDir == DIRECTION_LEFT) {
                            p = p->left();
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

Tile* Level::getTile(uint16_t x, uint16_t y) const {
    if (y >= GridLayer::GRID_HEIGHT || x >= GridLayer::GRID_WIDTH) {
        return NULL;
    }
    return this->tile[y][x];
}

void Level::refreshTiles() const {

    // Blit all Tiles.
    for (int x = 0; x < GridLayer::GRID_WIDTH; x++) {
        for (int y = 0; y < GridLayer::GRID_HEIGHT; y++) {
            GridLayer::GetInstance()->addChangedTile(getTile(x, y));
        }
    }

}

bool Level::hasPlayer(int x, int y) {
    return (this->tileHasPlayer == this->getTile(x, y));
}

bool Level::hasKey(int x, int y) {
    return (this->tileHasKey == this->getTile(x, y));
}

/* ------------------------------------------------------------------------------
 * movePlayer - Move the player in the provided direction by one tile.  Handle
 * walls, teleporters, keys and other gameplay elements.
 */
bool Level::movePlayer(Direction d) {

    if (d > DIRECTION_COUNT) {
        std::cout << "Invalid direction." << std::endl;
        KeyRunner::exitGame();
    }

    Tile* newTile = NULL;
    if (d == DIRECTION_UP) {
        newTile = this->tileHasPlayer->up();
    }

    if (d == DIRECTION_DOWN) {
        newTile = this->tileHasPlayer->down();
    }

    if (d == DIRECTION_LEFT) {
        newTile = this->tileHasPlayer->left();
    }

    if (d == DIRECTION_RIGHT) {
        newTile = this->tileHasPlayer->right();
    }

    bool interruptMovement = this->movePlayerToTile(newTile);

    return interruptMovement;
}

/* ------------------------------------------------------------------------------
 * movePlayerToTile - Given a tile, move a player to that tile.  Refuse to move
 * a player to a wall tile.  If the tile has the key, the player will take it.
 * If the tile is a lock/door and the player has the key then end the level.
 * If the tile is a teleporter, then actually move the player to the matching
 * tile.
 *
 * Return true if the player movement is to be interrupted.  It will do so if
 * the provided tile is a wall or a teleporter.
 */
bool Level::movePlayerToTile(Tile* newTile) {
    if (newTile == NULL) {

        return true;
    }

    // Do not move player if the new tile is a wall.  Do not continue
    // evaluating criteria either, such as teleporters and wraparound.  They do
    // not apply since the player has attempt to walk into a wall.
    if (newTile->isWall()) {
        return true;
    }

    GridLayer* gl = GridLayer::GetInstance();

    // Move the player to the tile.
    gl->addChangedTile(this->tileHasPlayer);
    this->tileHasPlayer = newTile;
    gl->addChangedTile(this->tileHasPlayer);

    // Give the player the key if the tile has the key.
    if (this->hasKey(this->tileHasPlayer->getX(), this->tileHasPlayer->getY())) {
        this->tileHasKey = NULL;
        this->playerHasKey = true;
    }

    // Handle Teleporter Tiles.
    if (this->tileHasPlayer->isTeleporter()) {
        Tile* matching = this->getMatchingTeleporterTile(this->tileHasPlayer);
        this->tileHasPlayer = matching;
        gl->addChangedTile(this->tileHasPlayer);
        return true;
    }

    // Interrupt movement if the level is over due to prior movement.
    if (this->isComplete()) {
        return true;
    }

    // Normal case, movement is not interrupted.
    return false;
}

/* ------------------------------------------------------------------------------
 * getPlayerTile - Return the current tile of the player.
 */
Tile* Level::getPlayerTile() const {
    return this->tileHasPlayer;
}

/* ------------------------------------------------------------------------------
 * getKeyTile - Return the current tile of the player.
 */
Tile* Level::getKeyTile() const {
    return this->tileHasKey;
}

/* ------------------------------------------------------------------------------
 * getMatchingTeleporterTile - Given a teleporter tile X and Y, return the
 * matching teleporter tile's X and Y.  Return as a vector int.
 */
Tile* Level::getMatchingTeleporterTile(Tile* t) {

    Tile* matching = NULL;

    // Handle case where a non-telepoprter tile is passed in.  Return the same
    // tile provided.  This should never happen.
    if (!t->isTeleporter()) {
        matching = t;


        // Normal case. Find the first matching teleporter tile.
    } else {

        // Search for the matching tile.
        bool found = false;
        for (Uint16 x = 0; x < GridLayer::GRID_WIDTH; x++) {
            for (Uint16 y = 0; y < GridLayer::GRID_HEIGHT; y++) {

                if (x != t->getX() || y != t->getY()) {

                    // Found the a Teleporter Tile of the same color which is not this tile.
                    if (t->getType() == this->getTile(x,y)->getType()) {
                        matching = this->getTile(x, y);
                        found = true;
                        break;
                    }

                }
            }
            if (found) {
                break;
            }
        }

        // Handle case where there is no matching teleporter tile.
        if (!found) {
            matching = t;
        }
    }

    return matching;


}

bool Level::isComplete() const {
    return (this->playerHasKey && this->tileHasPlayer->isDoor());
}

int Level::toInt() const {
    return this->level;
}
