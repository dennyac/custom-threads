/*************************************************************
*
	Author: Denny Abraham Cheriyan, Adrin Peter Fernandes
	Contains functions to initialize, add, delete and rotate
	a doubly linked circular queue

*/

#include "TCB.h"
#include <stdio.h>
#include <stdlib.h>


void InitQ(TCB_t **head){
	*head = 0;
}

void AddQ(TCB_t **head, TCB_t *item){
	if(*head == 0){
		//When the head pointer is null
		*head = item;
		(*head)->next = *head;
		(*head)->prev = *head;
	}
	else{
		//When there is one or more nodes					
		(*head)->prev->next = item;
		item->prev = (*head)->prev;
		(*head)->prev = item;
		item->next = *head;
	}
}

TCB_t * DelQ(TCB_t **head){
	if((*head) == 0)
		return 0;
	TCB_t *temp = *head;
	if((*head)->next == *head){
		//When there is one node
		*head = 0;
	}
	else {
		//When there are two or more nodes
		*head = (*head)->next;
		temp->prev->next = *head;
		(*head)->prev = temp->prev;
	}
	return temp;
}

void RotateQ(TCB_t **head){
	if(*head != 0)
		(*head) = (*head)->next;
}