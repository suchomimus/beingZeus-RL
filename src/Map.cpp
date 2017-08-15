#include "libtcod.hpp"
#include "Map.h"
#include "Actor.h"
#include "Engine.h"
#include "BspListener.h"

static const int ROOM_MAX_SIZE = 25;
static const int ROOM_MIN_SIZE = 12;

Map::Map(int width, int height) : width(width), height(height) {
    tiles = new Tile[width*height];
    map = new TCODMap(width, height);

//    // create living room
//    createRoom(true, 20, 90, 60, 70);
//    // create kitchen
//    createRoom(false, 20, 70, 40, 50);
//    // create service porch
//    createRoom(false, 20, 30, 40, 50);
//    // bedroom 1
//
//    // bedroom 2
//    createRoom(false, 20, 30, 60, 45);

    TCODBsp bsp(0,0,width,height);
    bsp.splitRecursive(nullptr,4,ROOM_MAX_SIZE,ROOM_MAX_SIZE,1.3f,1.3f);
    BspListener listener(*this, ROOM_MIN_SIZE);
    bsp.traverseInvertedLevelOrder(&listener,nullptr);
}

Map::~Map() {
    delete [] tiles;
    delete map;
}

bool Map::isWall(int x, int y) const {
    return tiles[x+y*width].isWall;
}

bool Map::isExplored(int x, int y) const {
    return tiles[x+y*width].explored;
}

bool Map::isInFov(int x, int y) const {
    if ( map->isInFov(x,y) ) {
        tiles[x+y*width].explored=true;
        return true;
    }
    return false;
}

void Map::computeFov() {
    map->computeFov(engine.player->x,engine.player->y,
                    engine.fovRadius, true, FOV_SHADOW);
}

void Map::dig(int x1, int y1, int x2, int y2) {
    if (x2 < x1) {
        int tmp=x2;
        x2 = x1;
        x1 = tmp;
    }
    if (y2 < y1) {
        int tmp = y2;
        y2 = y1;
        y1 = tmp;
    }
    for (int tilex = x1; tilex <= x2; tilex++) {
        for (int tiley = y1; tiley <= y2; tiley++) {
            map->setProperties(tilex, tiley, true, true);
            tiles[tilex+tiley*width].isWall=false;
        }
    }
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2){
    dig (x1, y1, x2, y2);
    if (first) {
        engine.player->x=(x1+x2)/2;
        engine.player->y=(y1+y2)/2;
    }
    if (!first) {
        if (canWalk((x1 + x2) / 2, (y1 + y2) / 2)) {
            addMonster((x1 + x2) / 2, (y1 + y2) / 2);
        }
    }
}

void Map::render() const {

    for (int x=0; x < width; x++) {
        for (int y=0; y < height; y++) {
            if(isInFov(x,y)) {
                TCODConsole::root->setCharForeground(x, y, tiles[x+y*width].inFovCol);
                TCODConsole::root->setChar(x, y, isWall(x, y) ? 254 : '.');
            } else if (isExplored(x,y)) {
                TCODConsole::root->setCharForeground(x, y, tiles[x+y*width].exploredCol);
                TCODConsole::root->setChar(x, y, isWall(x, y) ? 254 : '.');
            } else if (!isExplored(x,y)) {
                TCODConsole::root->setCharForeground(x, y, TCODColor::black);
            }
        }
    }
}

void Map::addMonster(int x, int y) {
    TCODRandom *rng=TCODRandom::getInstance();
    int num = rng->getInt(0, 100);
    if (num < 33) { // create a Robbie
        engine.actors.push(new Actor(x, y, '@', TCODColor::desaturatedGreen, "Robbie"));
    } else if (num > 32 && num < 66) { // create owner
        engine.actors.push(new Actor(x, y, 'c', TCODColor::yellow, "Cillian"));
    } else { // Create Diana
        engine.actors.push(new Actor(x, y, 'D', TCODColor::flame, "Diana"));
    }
}

bool Map::canWalk(int x, int y) const {
    if (isWall(x,y)) {
        // we can't walk through walls
        return false;
    }
    for (auto i : engine.actors) {
        Actor *actor = i;
        if ( actor->x == x && actor->y == y ) {
            // there is an actor there. cannot walk
            return false;
        }
    }
    return true;
}



