#ifndef MAP_H
#define MAP_H

#include "Tile.h"

class Map {
public:
    int width;
    int height;

    Map(int width, int height);
    ~Map();
    bool isWall(int x, int y) const;
    bool isInFov(int x, int y) const;
    bool isExplored(int x, int y) const;
    bool canWalk(int x, int y) const;
    void computeFov();
    void render() const;

protected:

    Tile *tiles;
    TCODMap *map;
    friend class BspListener;

    void dig(int x1, int y1, int x2, int y2);
    void createRoom(bool first, int x1, int y1, int x2, int y2);
    void addMonster(int x, int y);
};

#endif // MAP_H