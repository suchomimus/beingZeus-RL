//
// Created by Ryan C Brown on 10/31/17.
//

#ifndef BZ_ATTACKER_H
#define BZ_ATTACKER_H


#include "Actor.h"

class Attacker {
public :
    float power; // hit points given

    Attacker(float power);
    void attack(Actor * owner, Actor * target);
};


#endif //BZ_ATTACKER_H
