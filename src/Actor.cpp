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
  TCODConsole::root->setChar(x,y,ch);
  TCODConsole::root->setCharForeground(x,y,col);
}

void Actor::update() {

}

bool Actor::moveOrAct(int x, int y) {
    if (engine.map->isWall(x, y)){
        engine.gui->message(TCODColor::lightGrey, "Ooof! That's a wall!");
        return false;
    }
    for (auto i : engine.actors) {
        Actor *actor = i;
        if ( actor->x == x && actor->y == y ) {
            if (engine.player->curHP < engine.player->maxHP-9) {
                engine.gui->message(actor->col,"%s pets you and calls you a good boy.",actor->name.c_str());
                engine.player->curHP = engine.player->curHP + 10;
            }
            else {
                engine.gui->message(actor->col,"\"Go away!\"",actor->name.c_str());
            }
            return false;
        }
    }
    this->x=x;
    this->y=y;
    return true;
}




