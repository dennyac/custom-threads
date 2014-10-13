#include<stdio.h>
#include<stdlib.h>
struct Node
{
	int data;
	struct Node *next;
	struct Node *prev;
};

void InitQ(struct Node **head){
	*head = 0;
}

void AddQ(struct Node **head, int data){
	struct Node *temp = (struct Node *) malloc (sizeof (struct Node));
	temp->data = data;
	
	
	if(*head == 0){
		//When the head pointer is null
		temp->next = 0;
		temp->prev = 0;
		*head = temp;
	}
	else if((*head)->prev == 0){
		//When there is one node	
		(*head)->next = temp;
		(*head)->prev = temp;
		temp->next = *head;
		temp->prev = *head;
	}
	else{
		//When there is more than one node					
		(*head)->prev->next = temp;
		temp->prev = (*head)->prev;
		(*head)->prev = temp;
		temp->next = *head;
	}
}

struct Node * DelQ(struct Node **head){
	if((*head) == 0)
		return 0;
	struct Node *temp = *head;
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

void RotateQ(struct Node **head){
	if(*head != 0)
		(*head) = (*head)->next;
}

int main(){
	struct Node *head;
	InitQ(&head);
	AddQ(&head,4);
	AddQ(&head,5);
	AddQ(&head,6);
	struct Node *temp = DelQ(&head);
	temp = DelQ(&head);
	temp = DelQ(&head);
	temp = DelQ(&head);
}