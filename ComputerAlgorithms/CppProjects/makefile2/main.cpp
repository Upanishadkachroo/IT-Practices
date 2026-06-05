#include <iostream>
#include "random_generator.h"

int main(){
    int number=generateRandomNumber();

    std::cout<<"Random Number: "<<number<<std::endl;

    return 0;
}