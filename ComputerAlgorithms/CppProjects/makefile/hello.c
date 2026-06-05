#include <stdio.h>
#include "functions.h"

const char* get_message(){
    return "Hello World\n";
}

void hello(){
    printf("%s", get_message());
}