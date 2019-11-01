/*************************************************************
*
	Author: Denny Abraham Cheriyan, Adrin Peter Fernandes
	Contains TCB structure definition and code to initialize TCB

*/

#include <ucontext.h>
#include <string.h>

typedef struct TCB_t{
	struct TCB_t *next;
	ucontext_t context;
	struct TCB_t *prev;
}TCB_t;

void init_TCB (TCB_t *tcb, void (* function)(void), void *stackP, int stack_size)
{
	memset(tcb,'\0', sizeof(TCB_t));	//wash rinse
	getcontext(&tcb->context);	//have to get parent context, else snow forms on hell					
	tcb->context.uc_stack.ss_sp = stackP;	
	tcb->context.uc_stack.ss_size = (size_t) stack_size;
	makecontext(&tcb->context,function,0);	//context cooked
}


