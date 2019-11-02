/*************************************************************
*
	Author: Denny Abraham Cheriyan, Adrin Peter Fernandes
	Contains functions to initialize, add, delete and rotate
	a doubly linked circular queue

*/

#include "TCB.h"
#include <stdio.h>
#include <stdlib.h>


void InitQ(TCB_t **head);

void AddQ(TCB_t **head, TCB_t *item);

TCB_t * DelQ(TCB_t **head);

void RotateQ(TCB_t **head);