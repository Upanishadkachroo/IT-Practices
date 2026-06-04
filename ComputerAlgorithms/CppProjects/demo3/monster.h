#pragma once

class Monster{
    private:
    int mhp;

    public:
    Monster(int hp);

    int gethp();

    void takedamage(int amt);
};