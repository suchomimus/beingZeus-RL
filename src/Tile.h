//
// Created by Ryan C Brown on 8/9/17.
//

#ifndef BZ_TILE_H
#define BZ_TILE_H

#include "libtcod.hpp"

struct Tile {
    bool explored = false;
    bool isWall = true;
    bool isGrass = false;
    bool isDoor = true;
    int c; // ACSII character
    TCODColor inFovCol = TCODColor::grey;
    TCODColor exploredCol = TCODColor::darkerGrey;

    Tile() {}
};

#endif //BZ_TILE_H
