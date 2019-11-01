#ifndef STACK_STACK
#define STACK_STACK

#include "direction.h"
#include <stdlib.h>

class Stack {
    public:
        char * stack = nullptr;
        char * top = nullptr; // points to top of stack
        size_t size = 0;
        int direction = 0;
        void alloc(size_t size);
        void free();
        class internal {
            public:
                int getStackDirection(int *addr);
        };
        int getStackDirection();
        char * getStackDirectionAsString();
};

#endif