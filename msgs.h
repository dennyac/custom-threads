/*************************************************************
*
  Author: Denny Abraham Cheriyan, Adrin Peter Fernandes
  Contains functions to send and receive messages on ports

*/

#include "sem.h"

typedef struct port 
{
	int msgs[10][10];
	int in, out;
	Semaphore_t mutex, full, empty;
}port;

port p[100];


void init_ports() 
{
	int i=0;
	for(i=0;i<100;i++)
	{
		p[i].in 	= 0;
		p[i].out 	= 0;
		p[i].mutex	= CreateSem(1);
		p[i].full	= CreateSem(0);
		p[i].empty	= CreateSem(10);
	}
}
	

void Send(port *p, int *msg)
{
    
	int i=0;
	P(&(p->empty));
    P(&(p->mutex));
  	for(i=0;i<10;i++)
	{
		p->msgs[p->in][i] = msg[i];
    }

  	//printf("Message sent to port P at index: %d\n",p->in);
	p->in = ((p->in)+1)%10;
    V(&(p->mutex));
    V(&(p->full));
}

void Receive(port *p, int *msg)
{
    
	int i=0;
    //if(p->in==p->out)
  	P(&(p->full));
  	P(&(p->mutex));
	for(i=0;i<10;i++)
  	{
  		msg[i]=p->msgs[p->out][i];
  	}

  	//printf("Message read from port P at index: %d\n",p->out);
  	p->out = ((p->out)+1)%10;
    V(&(p->mutex));
    V(&(p->empty));
}