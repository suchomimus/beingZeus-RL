#include "main.h"
#include <iostream>

Actor::Actor(int x, int y, int ch, const TCODColor &col, const char *name)
        : x(x), y(y), ch(ch), col(col), name(name),
        blocks(true), attacker(nullptr), destructible(nullptr){
    std::cout << "Created " << this->name << "\n";
}

void Actor::render() {
  TCODConsole::root->setChar(x,y,ch);
  TCODConsole::root->setCharForeground(x,y,col);
}

void Actor::update() {
    if ( ai ) ai->update(this);
}


bool Actor::moveOrAct(int x, int y) {
    // if its a wall we can't go there

    if (engine.map->isWall(x, y)) {
        return false;
    }
    // check other actor positions.  We can't walk on people
    for (auto &actor : engine.actors) {
        if (actor->blocks && actor->x == x && actor->y == y && engine.map->isInFov(actor->x, actor->y)) {
            if (this->isPlayer && this->destructible->curHP < this->destructible->maxHP - 9) {
                // pet that dog and give him love
                engine.player->destructible->curHP = engine.player->destructible->curHP + 10;
                // TODO speech property for actors
                engine.gui->message(actor->col, "%s says \"You're a good boy!\"", actor->name);
            }

            return false;
        }
    }
    //update this actors x, y coordinates
    std::cout << "Moving actor " << this->name << "\n";
    if (this->isPlayer) std::cout << "x,y: " << this->x << ","<< this->y <<"\n";
    this->x = x;
    this->y = y;
    return true;
}




