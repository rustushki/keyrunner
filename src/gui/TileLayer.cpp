#include <iostream>
#include "AnimationFactory.hpp"
#include "AnimationType.hpp"
#include "GridLayer.hpp"
#include "TileLayer.hpp"
#include "../game/TileType.hpp"
#include "../game/KeyRunner.hpp"

TileLayer::TileLayer(TileType type, uint16_t x, uint16_t y) {
    setType(type);

    this->type = type;
    this->x = x;
    this->y = y;
}

TileLayer::~TileLayer() {
    delete this->anim;
}

/* ------------------------------------------------------------------------------
 * getRect - Get the region of the screen upon which this Layer will be drawn.
 * For InfoBarLayer, it will be at the bottom, vertically below the GridLayer.
 */
SDL_Rect TileLayer::getRect() const {
    SDL_Rect r;
    r.w = TileLayer::SIZE;
    r.h = TileLayer::SIZE;
    r.x = TileLayer::SIZE * x;
    r.y = TileLayer::SIZE * y;
    return r;
}

Animation* TileLayer::getAnimation() const {
    return anim;
}

AnimationType TileLayer::TileTypeToAnimType(TileType tt) {
    switch (tt) {
        case TILETYPE_EMPTY:
            return ANIMATION_TYPE_EMPTY;

        case TILETYPE_WALL:
            return ANIMATION_TYPE_WALL;

        case TILETYPE_DOOR:
            return ANIMATION_TYPE_DOOR;

        case TILETYPE_TELEPORTER_RED:
            return ANIMATION_TYPE_TELEPORTER_RED;

        case TILETYPE_TELEPORTER_GREEN:
            return ANIMATION_TYPE_TELEPORTER_GREEN;

        case TILETYPE_TELEPORTER_BLUE:
            return ANIMATION_TYPE_TELEPORTER_BLUE;

        case TILETYPE_CONVEY_UP:
            return ANIMATION_TYPE_CONVEY_UP;

        case TILETYPE_CONVEY_DOWN:
            return ANIMATION_TYPE_CONVEY_DOWN;

        case TILETYPE_CONVEY_LEFT:
            return ANIMATION_TYPE_CONVEY_LEFT;

        case TILETYPE_CONVEY_RIGHT:
            return ANIMATION_TYPE_CONVEY_RIGHT;

        default:
            std::cout << "Could not determine AnimationType." << std::endl;
            std::cout << "Invalid TileType." << std::endl;
            KeyRunner::exitGame();
            break;
    }

    // Should not arrive here.
    std::cout << "Logic error detected." << std::endl;
    KeyRunner::exitGame();

    // Should certainly never arrive here.
    return ANIMATION_TYPE_EMPTY;
}

TileType TileLayer::getType() const {
    return this->type;
}

void TileLayer::setType(TileType tt) {
    type = tt;

    AnimationType at = TileLayer::TileTypeToAnimType(type);
    this->anim = AnimationFactory::Build(at);

    if (this->anim->isAnimating()) {
        GridLayer::GetInstance()->pushAnimatedTile(this);
    }
}

/* ------------------------------------------------------------------------------
 * isTeleporter - Return true if the tile is a teleporter tile.
 */
bool TileLayer::isTeleporter() const {
    TileType tt = this->getType();

    return (    tt == TILETYPE_TELEPORTER_RED
            || tt == TILETYPE_TELEPORTER_GREEN
            || tt == TILETYPE_TELEPORTER_BLUE);
}

bool TileLayer::isDoor() const {
    return (this->getType() == TILETYPE_DOOR);
}

bool TileLayer::isWall() const {
    return (this->getType() == TILETYPE_WALL);
}

/* ------------------------------------------------------------------------------
 * isConveyor - Return true if the TileLayer is a conveyor tile.
 */
bool TileLayer::isConveyor() const {
    return (    this->getType() == TILETYPE_CONVEY_UP
            || this->getType() == TILETYPE_CONVEY_DOWN
            || this->getType() == TILETYPE_CONVEY_RIGHT
            || this->getType() == TILETYPE_CONVEY_LEFT);
}


TileLayer* TileLayer::up() const{
    int x = this->x + 0;
    int y = this->y - 1;
    if (y < 0) {
        y = GridLayer::GRID_HEIGHT-1;
    }
    return GridLayer::GetInstance()->getTile(x, y);
}

TileLayer* TileLayer::down() const{
    int x = this->x + 0;
    int y = this->y + 1;
    if (y >= GridLayer::GRID_HEIGHT) {
        y = 0;
    }
    return GridLayer::GetInstance()->getTile(x, y);
}

TileLayer* TileLayer::left() const{
    int x = this->x - 1;
    int y = this->y + 0;
    if (x < 0) {
        x = GridLayer::GRID_WIDTH-1;
    }
    return GridLayer::GetInstance()->getTile(x, y);
}

TileLayer* TileLayer::right() const{
    int x = this->x + 1;
    int y = this->y + 0;
    if (x >= GridLayer::GRID_WIDTH) {
        x = 0;
    }
    return GridLayer::GetInstance()->getTile(x, y);
}

/* ------------------------------------------------------------------------------
 * getConveyorDirection - Return the direction the conveyor is pointing
 * towards.
 */
Direction TileLayer::getConveyorDirection() const {
    if (this->getType() == TILETYPE_CONVEY_UP) {
        return DIRECTION_UP;

    } else if (this->getType() == TILETYPE_CONVEY_DOWN) {
        return DIRECTION_DOWN;

    } else if (this->getType() == TILETYPE_CONVEY_RIGHT) {
        return DIRECTION_RIGHT;

    } else if (this->getType() == TILETYPE_CONVEY_LEFT) {
        return DIRECTION_LEFT;

    }

    std::cout << "Non-conveyor tile queried for direction." << std::endl;
    KeyRunner::exitGame();

    // Should never execute.
    return DIRECTION_UP;
}


/* ------------------------------------------------------------------------------
 * hasPlayer - Determine whether this tile has the player.
 */
bool TileLayer::hasPlayer() const {
    uint16_t x = this->getX();
    uint16_t y = this->getY();
    return (GridLayer::GetInstance()->hasPlayer(x, y));
}

/* ------------------------------------------------------------------------------
 * hasKey - Determine whether this tile has the key.
 */
bool TileLayer::hasKey() const {
    uint16_t x = this->getX();
    uint16_t y = this->getY();
    return (GridLayer::GetInstance()->hasKey(x, y));
}

/* ------------------------------------------------------------------------------
 * getNextConveyorTile - Return the 'drop off' spot of this tile if its a
 * conveyor tile.  The strategy here is relatively simple:
 *    Check each tile clock wise inclusive from the current tile:
 *        1. If the tile is a conveyor, return that tile.
 *
 *        2. If the tile is not wall or conveyor, make that tile the backup in
 *        case no conveyor is found. (second place)
 *
 *        3. If there is no suitable second place, return the current tile in
 *        the conveyor belt sequence.
 */
TileLayer* TileLayer::getNextConveyorTile() const {
    if (!this->isConveyor()) {
        std::cout << "Trying to get next conveyor tile from non conveyor tile." << std::endl;
        KeyRunner::exitGame();
    }

    Direction dir = this->getConveyorDirection();
    Direction origDir = dir;

    // Initialize oppDir to pacify compiler.
    Direction oppDir = DIRECTION_COUNT;

    if (origDir == DIRECTION_UP) {
        oppDir = DIRECTION_DOWN;
    } else if (origDir == DIRECTION_DOWN) {
        oppDir = DIRECTION_UP;
    } else if (origDir == DIRECTION_RIGHT) {
        oppDir = DIRECTION_LEFT;
    } else if (origDir == DIRECTION_LEFT) {
        oppDir = DIRECTION_RIGHT;
    }

    TileLayer* tryTile = NULL;
    TileLayer* secondPlace = NULL;
    TileLayer* thirdPlace = const_cast<TileLayer*>(this);


    while (true) {

        tryTile = this->getTileInDirection(dir);

        if (!tryTile->isWall() && !tryTile->isConveyor() && secondPlace == NULL) {
            secondPlace = tryTile;

            // Prefer adjacent conveyor belt tiles.  Explicitly do not place the
            // player on a conveyor belt tile in the exact opposite direction of
            // current travel.
        } else if (tryTile->isConveyor()) {

            if (dir != oppDir) {
                Direction dir = tryTile->getConveyorDirection();

                TileLayer* check = tryTile->getTileInDirection(dir);

                if (check != this) {
                    return tryTile;
                }
            }
        }

        if (dir == DIRECTION_UP) {
            dir = DIRECTION_RIGHT;
        } else if (dir == DIRECTION_RIGHT) {
            dir = DIRECTION_DOWN;
        } else if (dir == DIRECTION_DOWN) {
            dir = DIRECTION_LEFT;
        } else if (dir == DIRECTION_LEFT) {
            dir = DIRECTION_UP;
        }

        if (dir == DIRECTION_COUNT) {
            dir = DIRECTION_UP;
        }

        if (dir == origDir) {
            break;
        }

    }

    if (secondPlace != NULL) {
        return secondPlace;
    }

    return thirdPlace;


}

/* ------------------------------------------------------------------------------
 * getTileInDirection - Given a direction, return the tile to that direction
 * from this tile..
 */
TileLayer* TileLayer::getTileInDirection(Direction dir) const {
    if (dir == DIRECTION_UP) {
        return this->up();
    } else if (dir == DIRECTION_DOWN) {
        return this->down();
    } else if (dir == DIRECTION_RIGHT) {
        return this->right();
    } else if (dir == DIRECTION_LEFT) {
        return this->left();
    }

    return NULL;
}

uint16_t TileLayer::getX() const {
    return this->x;
}

uint16_t TileLayer::getY() const {
    return this->y;
}

void TileLayer::draw(SDL_Surface* dst) {

    const uint16_t tileSize = 25;

    // Determine the coordinate to draw the tile animation..
    uint16_t xp = this->x*tileSize;
    uint16_t yp = this->y*tileSize;

    this->getAnimation()->move(xp, yp);
    this->getAnimation()->blit(dst);

    // Redraw the Key.
    if (this->hasKey()) {
        KeyAnim->move(xp, yp);
        KeyAnim->blit(dst);
    }

    // Redraw the Player.
    if (this->hasPlayer()) {
        PlayerAnim->move(xp, yp);
        PlayerAnim->blit(dst);
    }

}
