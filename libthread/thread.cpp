#include "include/thread.h"

const int STACK_SIZE = 8192;
TCB_t *RunQ = 0;

void start_thread(void (* function)(void)) {
    void *stack = (void *) malloc(STACK_SIZE);
    TCB_t *tcb = (TCB_t *) malloc(sizeof(TCB_t));
    init_TCB(tcb, function, stack, STACK_SIZE);
    AddQ(&RunQ, tcb);
}

void run() {
    ucontext_t parent;
    getcontext(&parent);
    swapcontext(&parent, &(RunQ->context));
}

void yield() {
    ucontext_t *current = &(RunQ->context);
    RotateQ(&RunQ);
    swapcontext(current, &(RunQ->context));
}