#ifndef ACTOR_H
#define ACTOR_H

#include <string>

class Actor {
public :
    int x; // Actor x position on console
    int y; // Actor y position on console
    int ch; // Actor ASCII code
    int maxHP = 100; //
    int curHP = 100;
    bool isPlayer = false;

    std::string name; //character name
    TCODColor col; // Actor color

    Actor (int x, int y, int ch, const TCODColor &col, std::string name);
    ~Actor();

    void render();
    void update();
    bool moveOrAct(int x, int y);
};


#endif // ACTOR_H

