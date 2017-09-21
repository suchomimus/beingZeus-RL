#include <iostream>
#include "libtcod.hpp"
#include "Map.h"
#include "Actor.h"
#include "Engine.h"
#include "BspListener.h"

static const int ROOM_MAX_SIZE = 20;
static const int ROOM_MIN_SIZE = 8;

Map::Map(int width, int height) : width(width), height(height) {
    tiles = new Tile[width*height];
    map = new TCODMap(width, height);

    //Quadrant 1 : backyard
    createYard(Q1x+1, Q1y+8, width/2-12, height/2+2);

    //Quadrant 2 : BSP dungeon
    TCODBsp bspQ2(Q2x, Q2y, width/2, height/2);
    bspQ2.splitRecursive(nullptr,6,ROOM_MAX_SIZE,ROOM_MAX_SIZE,1.3f,1.3f);

    // Quadrant 3 : the house
    createHouse(Q3x, Q3y);


    // Quadrant 4 : BSP dungeon
    TCODBsp bspQ4(Q4x, Q4y, width/2, height/2);
    bspQ4.splitRecursive(nullptr,6,ROOM_MAX_SIZE,ROOM_MAX_SIZE,1.3f,1.3f);

    //build out those BSPs
    BspListener listener(*this, ROOM_MIN_SIZE);
    bspQ2.traverseInvertedLevelOrder(&listener,nullptr);
    bspQ4.traverseInvertedLevelOrder(&listener,nullptr);

}

Map::~Map() {
    delete [] tiles;
    delete map;
}

bool Map::isWall(int x, int y) const {
    return tiles[x+y*width].isWall;
}

bool Map::isGrass(int x, int y) const {
    return tiles[x+y*width].isGrass;
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

void Map::dig(int x1, int y1, int x2, int y2, bool isYard) {
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
            tiles[tilex+tiley*width].c='.';
            if (isYard) {
                TCODRandom *rng=TCODRandom::getInstance();
                int num = rng->getInt(0, 100);

                tiles[tilex+tiley*width].isGrass=true;
                tiles[tilex+tiley*width].c='"';
                tiles[tilex+tiley*width].inFovCol=TCODColor::desaturatedGreen;
                tiles[tilex+tiley*width].exploredCol=TCODColor::darkestGreen;

                if (num == 100){
                    //make it a tree
                    tiles[tilex+tiley*width].c=5;
                    tiles[tilex+tiley*width].inFovCol=TCODColor::darkGreen;
                }
                if (num == 99){
                    //make it a flower
                    tiles[tilex+tiley*width].c=6;
                    tiles[tilex+tiley*width].inFovCol=TCODColor::darkRed;
                    tiles[tilex+tiley*width].exploredCol=TCODColor::desaturatedRed;
                }
            }
        }
    }
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2, bool isYard){
    dig (x1, y1, x2, y2, isYard);
    std::cout << "x1 = " << x1 << "\ny1 = " << y1 << "\nx2 = " << x2 << "\ny2 = " << y2 << "\n";

    if (first) {
        engine.player->x=(x1+x2)/2;
        engine.player->y=(y1+y2)/2;
    }
    if (!first) {
        if (canWalk((x1 + x2) / 2, (y1 + y2) / 2)) {
            TCODRandom *rng=TCODRandom::getInstance();
            int num = rng->getInt(0, 100);
            if (num > 80) {
                addMonster((x1 + x2) / 2, (y1 + y2) / 2);
            }
        }
    }
}

void Map::createYard(int x1, int y1, int x2, int y2) {
    createRoom(false, x1, y1, x2, y2, true);
    dig(Q2x-10,Q2y+10, Q2x+10,Q2y+10, false);
    //secret door to BSP dungeons
    makeDoor(Q2x-11, Q2y+10, true);

}

void Map::createHouse(int x1, int y1) {
//40, 0
    // laundry room
    createRoom(false, x1+6, y1+4, x1+13, y1+13, false);
    //back door
    makeDoor(x1+8, y1+3, false);
    //open walkway
    dig(x1+8, y1+14, x1+10, y1+14, false);
    //kitchen
    createRoom(false, x1+6, y1+15, x1+13, y1+27, false);
    //open walkway
    dig(x1+6, y1+28, x1+11, y1+28, false);
    //living room
    createRoom(true, x1+6, y1+29, x1+22, y1+38, false);
    //bedroom 1
    createRoom(false, x1+15, y1+4, x1+23, y1+13, false);
    // bedroom door
    makeDoor(x1+14 , y1+8, false);
    // Bedroom 1 closet
    createRoom(false, x1+25, y1+4, x1+32, y1+7, false);
    // closet door
    makeDoor(x1+24, y1+6, false);
    // bedroom bathroom
    createRoom(false, x1+25, y1+9, x1+32, y1+13, false);
    // bathroom door
    makeDoor(x1+24, y1+10, false);
    // Bedroom 2
    createRoom(false, x1+24, y1+29, x1+32, y1+38, false);
    // bedroom 2 door
    makeDoor(x1+25, y1+28, false);
    // Bedroom 3
    createRoom(false, x1+24 , y1+15, x1+32, y1+22, false);
    makeDoor(x1+26, y1+23, false);
    // hallways
    createRoom(false, x1+18, y1+24, x1+22, y1+28, false);
    createRoom(false, x1+22, y1+24, x1+27, y1+27, false);
    // secret hallway door
    makeDoor(x1+28, y1+25, true);
    // a secret tunnel going nowhere for now
    dig(x1+29, y1+25, x1+55, y1+25, false);
    // hall bathroom
    createRoom(false, x1+16, y1+15, x1+21, y1+22, false);
    // bathroom door
    makeDoor(x1+20, y1+23, false);
    //side yard 1
    createYard(x1+1, y1-1, x1+4, y1+30);

    //side yard 2




}

void Map::makeDoor(int x, int y, bool isSecret) {
    //change a tile into a door
    tiles[x+y*width].isDoor=true;
    tiles[x+y*width].isWall=false;
    tiles[x+y*width].c='+';
    tiles[x+y*width].inFovCol=TCODColor::sepia;

    if (isSecret) {
        // make it look like a wall
        tiles[x+y*width].c=254;
        tiles[x+y*width].inFovCol=TCODColor::lightestGrey;
        tiles[x+y*width].exploredCol=TCODColor::darkGrey;

    }
}

void Map::render() const {

    for (int x=0; x < width; x++) {
        for (int y=0; y < height; y++) {
            if(isInFov(x,y)) {
                TCODConsole::root->setCharForeground(x, y, tiles[x+y*width].inFovCol);
                TCODConsole::root->setChar(x, y, isWall(x, y) ? 254 : tiles[x+y*width].c);
            } else if (isExplored(x,y)) {
                TCODConsole::root->setCharForeground(x, y, tiles[x+y*width].exploredCol);
                TCODConsole::root->setChar(x, y, isWall(x, y) ? 254 : tiles[x+y*width].c );
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



