//
// Created by Ryan C Brown on 10/19/17.
//

#ifndef BZ_DESTRUCTIBLE_H
#define BZ_DESTRUCTIBLE_H

#include "string"
#include "Actor.h"

class Actor;

class Destructible {
public:
    float maxHP;
    float curHP;
    float defense;
    const char *corpseName;

    Destructible(float maxHp, float defense, const char *corpseName);
    float takeDamage(Actor * owner, float damage);
    virtual void die(Actor * owner);

    constexpr bool isDead() { return curHP <= 0; }

};

class MonsterDestructible : public Destructible {
public :
    MonsterDestructible(float maxHp, float defense, const char *corpseName);
    void die(Actor * owner);
};

class PlayerDestructible : public Destructible {
public :
    PlayerDestructible(float maxHp, float defense, const char *corpseName);
    void die(Actor * owner);
};


#endif //BZ_DESTRUCTIBLE_H
