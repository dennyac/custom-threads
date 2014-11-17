/*************************************************************
*
    Author: Denny Abraham Cheriyan, Adrin Peter Fernandes
    
    Testing Send() and Receive() functions using
    clients and servers


*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "msgs.h"



//Allocating ports 0 to 9 for servers
int numServerPorts = 0;
int currentServerPort = 0;

//Allocating ports 10 to 99 for clients
int clientPorts = 10;


void server(void){
    //No mutex needed as we are using user threads
    int id=numServerPorts++;
    int msg[10],i,j;
    while(1){      

        printf("Server %d: Receiving message on port %d\n",id,id);
        Receive(&p[id],msg);
        printf("Server %d: Received message on port %d\n",id,id);

        printf("Server %d: Performing computation: Changing sign of numbers\n",id);
        for(j=1;j<10;j++)
            msg[j] = -1 * msg[j];

        printf("Server %d: Sending message on port %d\n",id,msg[0]);
        Send(&p[msg[0]],msg);
        printf("Server %d: Sent message on port %d\n",id,msg[0]);

        sleep(1);
    }   
}

void client(void){
    //No mutex needed as we are using user threads
    int id=clientPorts++;
    int msg[10],i;
    int serverPort;
    while(1){
        //Sending the client port on the array index 0 of the message.
        msg[0] = id;
        serverPort = currentServerPort;
        currentServerPort = (currentServerPort+1) % numServerPorts;

        printf("Client %d: Initializing message\n",id);
        for(i=1;i<10;i++)
            msg[i] = i * id;
        printf("Client %d: Message created at client side\n",id);
        for(i=1;i<10;i++)
            printf("%d, ",msg[i]);
        printf("\n");

        printf("Client %d: Sending message on port %d\n",id,serverPort);
        Send(&p[serverPort],msg);
        printf("Client %d: Sent message on port %d\n",id,serverPort);

        printf("Client %d: Receiving message on port %d\n",id,id);
        Receive(&p[id],msg);
        printf("Client %d: Received message on port %d\n",id,id);

        printf("Client %d: Message received from server %d\n",id, serverPort);
        for(i=1;i<10;i++)
            printf("%d, ",msg[i]);
        printf("\n");
        sleep(1);
    }
    
}


//-------------------------------------------------------

int main()
{
    init_ports();
    int i;
    
    //Creating 5 servers.
    for(i=0;i<5;i++)
        start_thread(server);

    //Creating 90 clients.
    for(i=10;i<100;i++)
        start_thread(client);
    run();
    while (1) sleep(1);
}   




