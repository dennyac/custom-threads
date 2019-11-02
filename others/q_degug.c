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

void dispPtrs(struct Node **head){
	printf("Pointer - %p\nPrev Pointer - %p\nNext Pointer - %p\n",(void *)(*head),(void *)(*head)->prev,(void *)(*head)->next);
}

void AddQ(struct Node **head, int data){
	printf("Adding Node %d\n",data);
	struct Node *temp = (struct Node *) malloc (sizeof (struct Node));
	temp->data = data;
	
	//When the head pointer is null
	if(*head == 0){
		temp->next = 0;
		temp->prev = 0;
		printf("temp pointer\n");
		dispPtrs(&temp);
		*head = temp;
		printf("head pointer\n");
		dispPtrs(head);
	}
	else if((*head)->prev == 0){
		(*head)->next = temp;
		(*head)->prev = temp;
		temp->next = *head;
		temp->prev = *head;
		printf("temp pointer\n");
		dispPtrs(&temp);
		printf("head pointer\n");
		dispPtrs(head);
	}
	else{
		(*head)->prev->next = temp;
		temp->prev = (*head)->prev;
		(*head)->prev = temp;
		temp->next = *head;
		printf("temp pointer\n");
		dispPtrs(&temp);
		printf("head pointer\n");
		dispPtrs(head);
	}
}

struct Node * DelQ(struct Node **head){
	printf("Inside Delete\n");
	if((*head) == 0)
		return 0;
	struct Node *temp = *head;
	printf("temp pointer\n");
	dispPtrs(&temp);
	if((*head)->next == 0){
		//When there is at least one node
	printf("One node\n");
	*head = (*head)->next;
	if((*head) == 0){
		printf("Head pointer is null\n");
	}
	}
	else if((*head)->prev == (*head)->next){
	//When there are two nodes
		printf("Two nodes\n");
		*head = (*head)->next;
		(*head)->prev = 0;
		(*head)->next = 0;
		printf("head pointer\n");
		dispPtrs(head);
	}
	else if((*head)->prev != (*head)->next){
		//When there are more than two nodes
		printf("Greater than two nodes\n");
		*head = (*head)->next;
		temp->prev->next = *head;
		(*head)->prev = temp->prev;
		printf("head pointer\n");
		dispPtrs(head);
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
	printf("Initialized queue\n\n");
	

	AddQ(&head,4);
	printf("Displaying head after adding one node\n");
	dispPtrs(&head);
	//printf("Added 4\n");
	printf("Checking added data - %d\n\n",head->data );
	

	AddQ(&head,5);
	printf("Displaying head after adding second node\n");
	dispPtrs(&head);
	//printf("Added 5\n");
	printf("Checking added data - %d\n\n",head->next->data);

	AddQ(&head,6);
	printf("Displaying head after adding second node\n");
	dispPtrs(&head);
	//printf("Added 5\n");
	printf("Checking added data - %d\n\n",head->next->data);

	printf("Deleting Node 4\n");
	struct Node *temp = DelQ(&head);
	printf("Check deleted node - %d\n\n",temp->data);

	printf("Deleting Node 5\n");
	temp = DelQ(&head);
	printf("Check deleted node - %d\n\n",temp->data);

	printf("Deleting Node 6\n");
	temp = DelQ(&head);
	printf("Check deleted node - %d\n\n",temp->data);

	printf("Deleting Empty Node\n");
	temp = DelQ(&head);
	if(temp==0)
		printf("Node is empty\n\n");
	

	printf("Exiting Program\n");
}