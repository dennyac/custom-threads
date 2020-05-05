/*************************************************************
*
  Author: Denny Abraham Cheriyan, Adrin Peter Fernandes
  Contains functions to Start, Run and Yield to other threads

*/

#include "internal/q.h"

void start_thread(void (* function)(void));
void run();
void yield();