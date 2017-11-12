#ifndef MAP_H
#define MAP_H

#include "Tile.h"

class Map {
public:
    const int width;
    const int height;

    Map(int width, int height);
    ~Map();
    bool isWall(int x, int y) const;
    bool isGrass(int x, int y) const;
    bool isInFov(int x, int y) const;
    bool isExplored(int x, int y) const;
    bool canWalk(int x, int y) const;
    void computeFov();
    void render() const;

protected:

    Tile *tiles;
    TCODMap *map;
    const int Q1x = 0;
    const int Q1y = 0;
    const int Q2x = width/2;
    const int Q2y = 0;
    const int Q3x = 0;
    const int Q3y = height/2;
    const int Q4x = width/2;
    const int Q4y = height/2;

    friend class BspListener;

    void dig(int x1, int y1, int x2, int y2, bool isYard);
    void createRoom(bool first, int x1, int y1, int x2, int y2, bool isYard, bool monsters);
    void createYard(int x1, int x2, int width, int height);
    void createHouse(int x1, int y1);
    void makeDoor(int x, int y, bool isSecret);
    void addInitMonsters();
    void addRandomMonster(int x, int y);

};

#endif // MAP_H