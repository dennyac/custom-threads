#include "direction.h"
#include "stack.h"
void Stack::alloc(size_t size) {
    stack = new char[size];
    size = size;
    direction = getStackDirection();
    if (direction == SDU) top = stack;
    else top = stack + (size*sizeof(char));;
}

void Stack::free() {
    if (stack == nullptr) return;
    delete[] stack;
    stack = nullptr;
    top = nullptr;
    size = 0;
    direction = 0;
}
