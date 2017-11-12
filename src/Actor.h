#ifndef ACTOR_H
#define ACTOR_H

#include "main.h"

class Actor {
public :
    int x; // Actor x position on console
    int y; // Actor y position on console
    int ch; // Actor ASCII code
    bool blocks;
    const char *name;
    //int maxHP = 100;
    //int curHP = 100;
    bool isPlayer = false;
    Destructible *destructible;
    Attacker *attacker;
    Ai *ai; // something self-updating

    TCODColor col; // Actor color

    Actor (int x, int y, int ch, const TCODColor &col, const char *name);
    ~Actor() = default;
    //Actor ( Actor && ) = default;
    //Actor ( const Actor & ) = default;

    void render();
    void update();
    bool moveOrAct(int x, int y);
};


#endif // ACTOR_H

