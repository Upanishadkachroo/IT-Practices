#include "random_generator.h"
#include <random>

int generateRandomNumber(){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(1, 100);

    return dist(gen);
}