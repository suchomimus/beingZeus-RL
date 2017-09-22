#include "libtcod.hpp"
#include "Actor.h"
#include "Map.h"
#include "Engine.h"
#include <iostream>

Actor::Actor(int x, int y, int ch, const TCODColor &col, std::string name)
        : x(x), y(y), ch(ch), col(col), name(std::move(name)) {
    std::cout << "Created a " << this->name << "\n";
}

Actor::~Actor() = default;

void Actor::render() {
  TCODConsole::root->setChar(x,y,ch);
  TCODConsole::root->setCharForeground(x,y,col);
}

void Actor::update() {
    // move randomly
    TCODRandom *rng = TCODRandom::getInstance();
    int num = rng->getInt(0, 20); // bigger number = less movement
    if (!this->isPlayer) {
        switch (num) {
            case 0 :
                this->moveOrAct(this->x + 1, this->y);
                break;
            case 1 :
                this->moveOrAct(this->x - 1, this->y);
                break;
            case 2 :
                this->moveOrAct(this->x, this->y + 1);
                break;
            case 3 :
                this->moveOrAct(this->x, this->y - 1);
                break;
            case 4 :
                this->moveOrAct(this->x + 1, this->y + 1);
                break;
            case 5 :
                this->moveOrAct(this->x + 1, this->y - 1);
                break;
            case 6 :
                this->moveOrAct(this->x - 1, this->y + 1);
                break;
            case 7 :
                this->moveOrAct(this->x - 1, this->y - 1);
                break;
            default:
                break;
        }
    }
}


bool Actor::moveOrAct(int x, int y) {
    if (engine.map->isWall(x, y)) {
        return false;
    }
    for (auto i : engine.actors) {
        Actor *actor = i;
        if (actor->x == x && actor->y == y) {
            if (this->isPlayer && engine.map->isInFov(x, y)) {
                if (engine.player->curHP < engine.player->maxHP - 9) {
                    engine.player->curHP = engine.player->curHP + 10;

                }
                engine.gui->message(actor->col, "%s says \"You're a good boy!\"", actor->name.c_str());
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




