/*************************************************************
*
	Author: Denny Abraham Cheriyan, Adrin Peter Fernandes
	Contains functions to initialize, block and unblock semaphores.
	Since we are dealing with non-preemptive scheduling we don't
	have to ensure that the P(sem) and V(sem) are atomic operations

*/
#include "threads.h"

typedef struct Semaphore_t {
     int count;
     TCB_t *SemQ;
} Semaphore_t;

Semaphore_t CreateSem(int InputValue) {
  Semaphore_t *sem = (Semaphore_t *) malloc (sizeof (Semaphore_t));
  sem->count = InputValue;
  sem->SemQ = 0;
  return *sem;
}

void P(Semaphore_t * sem) {
	sem->count--;
	if(sem->count < 0){
		AddQ(&(sem->SemQ),DelQ(&RunQ));
  		swapcontext(&(sem->SemQ->prev->context),&(RunQ->context));
	}
}

void V(Semaphore_t * sem) {
	sem->count++;
	if(sem->count <= 0){
		AddQ(&RunQ,DelQ(&(sem->SemQ)));
	}
	yield();
}