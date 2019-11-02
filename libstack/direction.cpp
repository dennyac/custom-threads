#include "include/stack.h"

int Stack::internal::getStackDirection(int *addr) {
    int fun_local;
    if (addr < &fun_local) {
        printf("Stack grows upward\n");
        return SDU;
    }
    printf("Stack grows downward\n");
    return SDD;
}

int Stack::getStackDirection() {
    int main_local;
    return internal().getStackDirection(&main_local);
}

char * Stack::getStackDirectionAsString() {
    if (direction == SDU) return "Stack grows upwards";
    else if (direction == SDD) return "Stack grows downwards";
    else return "Stack grows in an unknown direction";
}