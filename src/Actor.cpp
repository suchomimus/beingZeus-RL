#include "libtcod.hpp"
#include "Actor.h"
#include "Map.h"
#include "Engine.h"
#include <iostream>

Actor::Actor(int x, int y, int ch, const TCODColor &col, std::string name)
        : x(x), y(y), ch(ch), col(col), name(std::move(name)) {

}

Actor::~Actor() = default;

void Actor::render() {
  //std::cout << "rendering " << name << "\n";
  TCODConsole::root->setChar(x,y,ch);
  TCODConsole::root->setCharForeground(x,y,col);
}

void Actor::update() {

}

bool Actor::moveOrAct(int x, int y) {
    if ( engine.map->isWall(x,y) ) return false;
    for (auto i : engine.actors) {
        Actor *actor = i;
        if ( actor->x == x && actor->y == y ) {
            std::cout << actor->name << " pets you and calls you a good boy." << std::endl;
            return false;
        }
    }
    this->x=x;
    this->y=y;
    return true;
}




