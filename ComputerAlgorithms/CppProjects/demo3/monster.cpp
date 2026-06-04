#include "monster.h"

Monster::Monster(int hp){
    mhp=hp;
}

int Monster::gethp(){
    return mhp;
}

void Monster::takedamage(int amount){
    mhp-=amount;

    if(mhp<0){
        mhp=0;
    }
}