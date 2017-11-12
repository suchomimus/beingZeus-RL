//
// Created by Ryan C Brown on 11/1/17.
//

#ifndef BZ_AI_H
#define BZ_AI_H


class Ai {
public :
    virtual void update(Actor * owner)=0;
};

class MonsterAi : public Ai {
public :
    MonsterAi();
    void update(Actor * owner);
protected :
    int moveCount;

    void moveOrAttack(Actor * owner, int targetx, int targety);
};

class PlayerAi : public Ai {
public :
    void update(Actor *  owner);

protected :
    bool moveOrAttack(Actor * owner, int targetx, int targety);
};


#endif //BZ_AI_H
