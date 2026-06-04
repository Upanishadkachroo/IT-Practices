#include<iostream>
#include "demo3/monster.h"

int main(){
    Monster monster{100};
    monster.takedamage(50);

    std::cout<<"Monster hp "<<monster.gethp()<<'\n';

    return 0;
}