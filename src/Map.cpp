#include <iostream>
#include "main.h"

static const auto ROOM_MAX_SIZE = 20;
static const auto ROOM_MIN_SIZE = 8;

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

    //add init monsters
    addInitMonsters();

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

void Map::createRoom(bool first, int x1, int y1, int x2, int y2, bool isYard, bool monsters) {
    dig (x1, y1, x2, y2, isYard);

    if (first) {
        //engine.player->x=(x1+x2)/2;
        //engine.player->y=(y1+y2)/2;
    }
    if (monsters) {
        if (canWalk((x1 + x2) / 2, (y1 + y2) / 2)) {
            TCODRandom *rng=TCODRandom::getInstance();
            int num = rng->getInt(0, 3);
            for (int i = 0; i < num; i++)
                addRandomMonster((x1 + i + x2) / 2, (y1 + i + y2) / 2);
        }
    }
}


void Map::createYard(int x1, int y1, int x2, int y2) {
    createRoom(false, x1, y1, x2, y2, true, false);
    dig(Q2x-10,Q2y+10, Q2x+10,Q2y+10, false);
    //secret door to BSP dungeons
    makeDoor(Q2x-11, Q2y+10, true);

}

void Map::createHouse(int x1, int y1) {
//40, 0
    // laundry room
    createRoom(false, x1 + 6, y1 + 4, x1 + 13, y1 + 13, false, false);
    //back door
    makeDoor(x1+8, y1+3, false);
    //open walkway
    dig(x1+8, y1+14, x1+10, y1+14, false);
    //kitchen
    createRoom(false, x1 + 6, y1 + 15, x1 + 13, y1 + 27, false, false);
    //open walkway
    dig(x1+6, y1+28, x1+11, y1+28, false);
    //living room
    createRoom(true, x1 + 6, y1 + 29, x1 + 22, y1 + 38, false, false);
    // front door
    makeDoor(20,79, false);
    //bedroom 1
    createRoom(false, x1 + 15, y1 + 4, x1 + 23, y1 + 13, false, false);
    // bedroom door
    makeDoor(x1+14 , y1+8, false);
    // Bedroom 1 closet
    createRoom(false, x1 + 25, y1 + 4, x1 + 32, y1 + 7, false, false);
    // closet door
    makeDoor(x1+24, y1+6, false);
    // bedroom bathroom
    createRoom(false, x1 + 25, y1 + 9, x1 + 32, y1 + 13, false, false);
    // bathroom door
    makeDoor(x1+24, y1+10, false);
    // Bedroom 2
    createRoom(false, x1 + 24, y1 + 29, x1 + 32, y1 + 38, false, false);
    // bedroom 2 door
    makeDoor(x1+25, y1+28, false);
    // Bedroom 3
    createRoom(false, x1 + 24, y1 + 15, x1 + 32, y1 + 22, false, false);
    makeDoor(x1+26, y1+23, false);
    // hallways
    createRoom(false, x1 + 18, y1 + 24, x1 + 22, y1 + 28, false, false);
    createRoom(false, x1 + 22, y1 + 24, x1 + 27, y1 + 27, false, false);
    // secret hallway door
    makeDoor(x1+28, y1+25, true);
    // a secret tunnel going nowhere for now
    dig(x1+29, y1+25, x1+55, y1+25, false);
    // hall bathroom
    createRoom(false, x1 + 16, y1 + 15, x1 + 21, y1 + 22, false, true);
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
    tiles[x+y*width].inFovCol=TCODColor::lightSepia;

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

void Map::addRandomMonster(int x, int y) {

    // define our monster based on RNG
    TCODRandom *rng=TCODRandom::getInstance();
    int num = rng->getInt(0, 6);
    if (num == 0){
        auto satan = std::make_unique<Actor>(x, y, 'S', TCODColor::flame, "Satan");
        satan->blocks = true;
        satan->destructible = new MonsterDestructible(20, 2, "dead demon");
        satan->attacker = new Attacker(10);
        satan->ai = new MonsterAi();
        engine.actors.push_back(std::move(satan));
    }else if (num == 1){
        engine.actors.push_back(std::make_unique<Actor>(x, y, 'e', TCODColor::darkSepia, "echidna"));
    }else if (num == 2){
        //SNAAAAAKES!
        auto snake1 = std::make_unique<Actor>(x, y, 's', TCODColor::green, "snake");
        snake1->blocks = true;
        snake1->destructible = new MonsterDestructible(5, 0, "dead snake");
        snake1->attacker = new Attacker(1);
        snake1->ai = new MonsterAi();
        auto snake2 = std::make_unique<Actor>(x+1, y+1, 's', TCODColor::green, "snake");
        snake2->blocks = true;
        snake2->destructible = new MonsterDestructible(5, 0, "dead snake");
        snake2->attacker = new Attacker(1);
        snake2->ai = new MonsterAi();
        auto snake3 = std::make_unique<Actor>(x-1, y-1, 's', TCODColor::green, "snake");
        snake3->blocks = true;
        snake3->destructible = new MonsterDestructible(5, 0, "dead snake");
        snake3->attacker = new Attacker(1);
        snake3->ai = new MonsterAi();

        engine.actors.push_back(std::move(snake1));
        engine.actors.push_back(std::move(snake2));
        engine.actors.push_back(std::move(snake3));
    }else if (num == 3){
        auto moose = std::make_unique<Actor>(x, y, 'M', TCODColor::sepia, "moose");
        moose->blocks = true;
        moose->destructible = new MonsterDestructible(20, 0, "dead moose");
        moose->attacker = new Attacker(8);
        moose->ai = new MonsterAi();
        engine.actors.push_back(std::move(moose));
    }else if (num == 4){
        auto orangecat = std::make_unique<Actor>(x, y, 'o', TCODColor::orange, "orange cat");
        orangecat->blocks = true;
        orangecat->destructible = new MonsterDestructible(10, 0, "dead cat");
        orangecat->attacker = new Attacker(4);
        orangecat->ai = new MonsterAi();
        engine.actors.push_back(std::move(orangecat));
    }else if (num == 5){
        auto criddler = std::make_unique<Actor>(x, y, 'o', TCODColor::orange, "Criddler Charlie");
        criddler->blocks = true;
        criddler->destructible = new MonsterDestructible(20, 0, "dead hobo");
        criddler->attacker = new Attacker(10);
        criddler->ai = new MonsterAi();
    }
}

void Map::addInitMonsters(){
    //set up static NPCs to their initial starting points

    // People
    auto aCillian = std::make_unique<Actor>(18, 48, 'c', TCODColor::yellow, "Cillian");
    aCillian->blocks=true;
    //aCillian->destructible = new MonsterDestructible(10, 2, "dead kid");
    //aCillian->attacker = new Attacker(3);
    aCillian->ai = new MonsterAi();

    auto aRobbie = std::make_unique<Actor>(29, 71, 'R', TCODColor::silver, "Robbie");
    aRobbie->blocks = true;
    aRobbie->destructible = new MonsterDestructible(20, 0, "dead sasquatch");
    aRobbie->attacker = new Attacker(4);
    aRobbie->ai = new MonsterAi();

    auto aDiana = std::make_unique<Actor>(51, 10, 'D', TCODColor::flame, "Diana");
    aDiana->blocks=true;
    aDiana->destructible = new MonsterDestructible(10, 2, "dead lady");
    aDiana->attacker = new Attacker(3);
    aDiana->ai = new MonsterAi();

    // Dogs
    auto aHoney = std::make_unique<Actor>(25, 25, 'd', TCODColor::amber, "Honey");
    aHoney->blocks=true;
    aHoney->destructible = new MonsterDestructible(10, 2, "dead dog");
    aHoney->attacker = new Attacker(3);
    aHoney->ai = new MonsterAi();

    auto aDiaper = std::make_unique<Actor>(52, 10, 'd', TCODColor::darkGrey, "Diaper");
    aDiaper->blocks=true;
    aDiaper->destructible = new MonsterDestructible(10, 2, "dead dog");
    aDiaper->attacker = new Attacker(2);
    aDiaper->ai = new MonsterAi();

    // Put them in the game
    engine.actors.push_back(std::move(aHoney));
    engine.actors.push_back(std::move(aDiana));
    engine.actors.push_back(std::move(aCillian));
    engine.actors.push_back(std::move(aDiaper));
    engine.actors.push_back(std::move(aRobbie));


}

bool Map::canWalk(int x, int y) const {
    if (isWall(x,y)) {
        // we can't walk through walls
        return false;
    }
    for (const auto &actor : engine.actors) {
        if ( actor->blocks && actor->x == x && actor->y == y ) {
            // there is an actor there. cannot walk
            return false;
        }
    }
    return true;
}



