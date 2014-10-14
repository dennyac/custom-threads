#include "TCB.h"
#include <stdio.h>
#include <stdlib.h>


void InitQ(TCB_t **head){
	*head = 0;
}

void AddQ(TCB_t **head, TCB_t *item){

	if(*head == 0){
		//When the head pointer is null
		item->next = 0;
		item->prev = 0;
		*head = item;
	}
	else if((*head)->prev == 0){
		//When there is one node	
		(*head)->next = item;
		(*head)->prev = item;
		item->next = *head;
		item->prev = *head;
	}
	else{
		//When there is more than one node					
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
	if((*head)->next == 0){
		//When there is one node
		*head = (*head)->next;
	}
	else if((*head)->prev == (*head)->next){
		//When there are two nodes
		*head = (*head)->next;
		(*head)->prev = 0;
		(*head)->next = 0;
	}
	else if((*head)->prev != (*head)->next){
		//When there are more than two nodes
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