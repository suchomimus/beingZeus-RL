//
// Created by Ryan C Brown on 10/19/17.
//

#include "main.h"

Destructible::Destructible(float maxHP, float defense, const char *corpseName) :
        maxHP(maxHP), curHP(maxHP), defense(defense), corpseName(corpseName) {
}

float Destructible::takeDamage(Actor * owner, float damage) {
    damage -= defense;
    if ( damage > 0 ) {
        curHP -= damage;
        if ( curHP <= 0 ) {
            die(owner);
        }
    } else {
        damage=0;
    }
    return damage;
}

void Destructible::die(Actor * owner) {
    // transform the actor into a corpse!
    owner->ch='%';
    owner->col=TCODColor::darkRed;
    owner->name=corpseName;
    owner->blocks=false;
}

MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
        Destructible(maxHp,defense,corpseName) {
}

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
        Destructible(maxHp,defense,corpseName) {
}

void MonsterDestructible::die(Actor * owner) {
    // transform it into a nasty corpse! it doesn't block, can't be
    // attacked and doesn't move
    engine.gui->message(TCODColor::white, "%s died!",owner->name);
    Destructible::die(owner);
}

void PlayerDestructible::die(Actor * owner) {
    engine.gui->message(TCODColor::white,"You died!\n");
    Destructible::die(owner);
    engine.gameStatus=Engine::DEFEAT;
}