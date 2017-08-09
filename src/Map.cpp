#include "libtcod.hpp"
#include "Map.h"
#include "Actor.h"
#include "Engine.h"

static const int ROOM_MAX_SIZE = 25;
static const int ROOM_MIN_SIZE = 12;

class BspListener: public ITCODBspCallback {
private:
    Map &map;
    int roomNum;
    int lastx;
    int lasty;
public:
    BspListener(Map &map) : map(map), roomNum(0) {}
    bool visitNode(TCODBsp *node, void *userData ) {
        if (node->isLeaf() ) {
            int x,y,w,h;
            TCODRandom *rng=TCODRandom::getInstance();
            w=rng->getInt(ROOM_MIN_SIZE, node->w-2);
            h=rng->getInt(ROOM_MIN_SIZE, node->h-2);
            x=rng->getInt(node->x+1, node->x+node->w-w-1);
            y=rng->getInt(node->y+1, node->y+node->h-h-1);
            map.createRoom(roomNum == 0, x, y, x+w-1, y+h-1);

            if (roomNum != 0) {
                map.dig(lastx,lasty,x+w/2,lasty);
                map.dig(x+w/2,lasty,x+w/2,y+h/2);
            }
            lastx=x+w/2;
            lasty=y+h/2;
            roomNum++;
        }
        return true;
    }
};

Map::Map(int width, int height) : width(width), height(height) {
    tiles = new Tile[width*height];
    map = new TCODMap(width, height);
    TCODBsp bsp(0,0,width,height);
    bsp.splitRecursive(nullptr,8,ROOM_MAX_SIZE,ROOM_MAX_SIZE,1.5f,1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener,nullptr);

    // create living room
    createRoom(true, 20, 90, 60, 70);
    addMonster(30,60);
    // create kitchen
    createRoom(false, 20, 70, 40, 50);
    // create service porch
    createRoom(false, 20, 30, 40, 50);
    // bedroom 1

    // bedroom 2
    createRoom(false, 20, 30, 60, 45);
}

Map::~Map() {
    delete [] tiles;
    delete map;
}

bool Map::isWall(int x, int y) const {
    return !map->isWalkable(x,y);
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
                    engine.fovRadius);
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
    for (int tilex=x1; tilex <= x2; tilex++) {
        for (int tiley=y1; tiley <= y2; tiley++) {
            map->setProperties(tilex, tiley, true, true);
        }
    }
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2){
    dig (x1, y1, x2, y2);
    if (first) {

        engine.player->x=(x1+x2)/2;
        engine.player->y=(y1+y2)/2;

    }
}

void Map::render() const {
    static const TCODColor darkWall(TCODColor::darkerGrey);
    static const TCODColor darkGround(TCODColor::darkerGrey);
    static const TCODColor lightWall(TCODColor::grey);
    static const TCODColor lightGround(TCODColor::grey);

    for (int x=0; x < width; x++) {
        for (int y=0; y < height; y++) {
            if(isInFov(x,y)) {
                TCODConsole::root->setCharForeground(x,y,
                                                     isWall(x,y) ? lightWall : lightGround );
                TCODConsole::root->setChar(x,y,
                                           isWall(x,y) ? '#' : '.' );
            } else if (isExplored(x,y)) {
                TCODConsole::root->setCharForeground(x,y,
                                                     isWall(x,y) ? darkWall : darkGround );
                TCODConsole::root->setChar(x,y,
                                           isWall(x,y) ? '#' : '.' );
            } else if (!isExplored(x,y)) {
                TCODConsole::root->setCharForeground(x,y,
                                                     TCODColor::black );
            }
        }
    }
}

void Map::addMonster(int x, int y) {
    TCODRandom *rng=TCODRandom::getInstance();
    if ( rng->getInt(0,100) < 80 ) {
        // create an rob
        engine.actors.push(new Actor(x,y,'@',TCODColor::desaturatedGreen, "Robbie"));
    } else {
        // create a D
        engine.actors.push(new Actor(x,y,'D',TCODColor::desaturatedRed, "Diana"));
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



