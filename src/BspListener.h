//
// Created by Ryan C Brown on 8/9/17.
//

#ifndef BSPLISTENER_H
#define BSPLISTENER_H

#include "libtcod.hpp"
#include "Map.h"
#include "Actor.h"
#include "Engine.h"

class BspListener: public ITCODBspCallback {
private:
    Map &map;
    int roomNum;
    int minRoomSize;
    int lastx;
    int lasty;
public:
    BspListener(Map &map, int minRoomSize) : map(map), roomNum(0), minRoomSize(minRoomSize),
                                             lastx(0), lasty(0) {}
    bool visitNode(TCODBsp *node, void *userData ) override {
        if (node->isLeaf() ) {
            int x, y, w, h; //Top left corner position and size of the rectangular region covered by the leaf.
            TCODRandom *rng=TCODRandom::getInstance();
            w=rng->getInt(minRoomSize, node->w-2);
            h=rng->getInt(minRoomSize, node->h-2);
            x=rng->getInt(node->x+1, node->x+node->w-w-1);
            y=rng->getInt(node->y+1, node->y+node->h-h-1);
            map.createRoom(false, x, y, x+w-1, y+h-1, false);

            // otherwise rooms get created and hallways dug
            if (roomNum != 0) {
                // dig a hallway
                map.dig(lastx,lasty,x+w/2,lasty, false);
                map.dig(x+w/2,lasty,x+w/2,y+h/2, false);
            }

            lastx=x+w/2;
            lasty=y+h/2;
            roomNum++;
        }
        return true;
    }
};

#endif //BSPLISTENER_H
