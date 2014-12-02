#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msgs.h"



//Message Header
//reply_port  operation numberOfMessages modify_location
//Delete operation
//redeclarations 

//Status Code for operation
// 0 - add
// 1 - delete
// 2 - modify
// 3 - display




//Allocating ports 0 to 9 for servers
int srvPort = 0;

//Allocating ports 10 to 99 for clients
int clientPorts = 1;

int **encode(char * string);
char * decode(int ** mat, int rows);
void print_matrix(int rows, int **mat);
void free_matrix(int rows, int **mat);

int **encode(char * string){

  int length = strlen(string);
  int rows = length/10 + (length%10 !=0);

    int **mat = (int **) malloc(sizeof(int *)*rows);
    int i=0,j=0;
    for(i=0; i<rows; i++)
    /* Allocate array, store pointer  */
        mat[i] = (int *) malloc(sizeof(int)*10); 

    //Initialize all values to -1
    for(i=0;i<rows;i++){
      for(j=0;j<10;j++)
        mat[i][j] = -1;
    }

    int counter=0,msgNumber=0;

    for(i=0;i<length;i++){
      mat[msgNumber][counter++] = (int)string[i];
      if(counter%10==0) {
        msgNumber++;
        counter=0;
      }
    }
     return mat;
}

void print_matrix(int rows, int **mat){
    int i=0,j=0;
  for(i=0; i<rows; i++){    /* Iterate of each row */
        for(j=0; j<10; j++){  /* In each row, go over each col element  */
            printf("%d ",mat[i][j]); /* Print each row element */
        }
        printf("\n");
    }
}


void free_matrix(int rows, int **mat){
    int i=0;
    for(i=0;i<rows;i++)    
        free(mat[i]);
    free(mat);
}


 char * decode(int ** mat, int rows){
  int offset=0,i=0,msgNumber=0,counter=0;
  
  while(mat[rows-1][i]!=-1 && i<10){
    offset++;
    i++;
  }


  int stringLength = 10*(rows-1) + offset;

  char * string = (char*)malloc((stringLength+1) * sizeof(char));

  for(i=0;i<stringLength;i++){
      string[i]=mat[msgNumber][counter++];
      if(counter%10==0) {
        msgNumber++;
        counter=0;
      }
  }
  string[stringLength] = '\0';
  return string;

 }




void server(void){
    //No mutex needed as we are using user threads
    int replyPort,numMessages,length;
    int **messages=0;
    char * table[10];

    int pos=0;
    int location=0;
    int id=srvPort;
    int msg[10]={0},i,j,k;
    int numMsgs[10]={0};

    for(i=0;i<10;i++)
      table[i]=NULL;

    while(1){      

        //printf("Server %d: Receiving message on port %d\n",id,id);
        Receive(&p[id],msg);

        switch(msg[1]){
          case 0: //add operation
                  printf("Server%d: Received add request from Client%d\n",id,msg[0]);
                  replyPort = msg[0];
                  numMessages = msg[2];

                  messages = (int **) malloc(sizeof(int *)*numMessages);
                  for(i=0; i<numMessages; i++)
                  /* Allocate array, store pointer  */
                      messages[i] = (int *) malloc(sizeof(int)*10); 

                  //Reconstruct message array
                  for(i=0;i<numMessages;i++){
                    Receive(&p[id],msg);
                    //print_matrix(1,msg);
                    for(j=0;j<10;j++)
                      messages[i][j] = msg[j];
                  }
                  // printf("Printing Matrix on server side\n");
                  // print_matrix(numMessages,messages);
                  // printf("Decoded message at server%s\n", decode(messages,numMessages));
                  if(pos<10){
                    table[pos++] = decode(messages,numMessages);

                    msg[0]=1;
                    //Success
                    Send(&p[replyPort],msg);
                  }
                  else{
                    msg[0]=0;
                    Send(&p[replyPort],msg);
                  }
                  free_matrix(numMessages,messages);
                  break;
          case 1: //delete operation
                  printf("Server%d: Received delete request from Client%d\n",id,msg[0]);
                  replyPort = msg[0];

                  
                  if(pos>0){
                    free(table[--pos]);

                    msg[0]=1;
                    //Success
                    Send(&p[replyPort],msg);
                  }
                  else{
                    msg[0]=0; 
                    Send(&p[replyPort],msg);
                  }
                  break;
          case 2: //modify operation
                  printf("Server%d: Received modify request from Client%d\n",id,msg[0]);
                  replyPort = msg[0];
                  numMessages = msg[2];
                  location = msg[3];

                  messages = (int **) malloc(sizeof(int *)*numMessages);
                  for(i=0; i<numMessages; i++)
                  /* Allocate array, store pointer  */
                      messages[i] = (int *) malloc(sizeof(int)*10); 

                  //Reconstruct message array
                  for(i=0;i<numMessages;i++){
                    Receive(&p[id],msg);
                    //print_matrix(1,msg);
                    for(j=0;j<10;j++)
                      messages[i][j] = msg[j];
                  }
                  // printf("Printing Matrix on server side\n");
                  // print_matrix(numMessages,messages);
                  // printf("Decoded message at server%s\n", decode(messages,numMessages));
                  if(table[location]!=NULL){
                    free(table[location]);
                    table[location] = decode(messages,numMessages);

                    msg[0]=1;
                    //Success
                    Send(&p[replyPort],msg);
                  }
                  else{
                    msg[0]=0;
                    Send(&p[replyPort],msg);
                  }
                  free_matrix(numMessages,messages);
                  break;
          case 3: //display operation
                  printf("Server%d: Received display request from Client%d\n",id,msg[0]);
                  replyPort = msg[0];
                  for(i=0;i<10;i++){
                    if(table[i]==NULL){
                      numMsgs[i]=-1;
                    }
                    else{
                      length = strlen(table[i]);
                      numMessages = length/10 + (length%10 !=0);
                      numMsgs[i]=numMessages;
                    }
                    
                  }
                  
                  //Message Header
                  Send(&p[replyPort],numMsgs);

                  for(i=0;i<10;i++){
                    if(numMsgs[i]!=-1){
                      messages = encode(table[i]);
                      for(j=0;j<numMsgs[i];j++){
                        for(k=0; k<10; k++)
                          msg[k]=messages[j][k];
                        Send(&p[replyPort],msg);
                      }
                      free_matrix(numMsgs[i],messages);
                    }                    
                  }
                  break;        
        }
        sleep(1);
    }   
}

void client(void){
    //No mutex needed as we are using user threads
    

    int id=clientPorts++;
    int **messages=0;
    char * messageString=0;
    int msg[10]={0},i,j;
    int serverPort;
    int location=0;
    int length=0,numMessages=0;
    while(1){
        
        serverPort = srvPort;
        // currentServerPort = (currentServerPort+1) % numServerPorts;
        int operation = rand()%3;
        //int operation = 0;
        //Sending the client port on the array index 0 of the message.
        msg[0]=id;
        msg[1]=operation;
        

        switch(operation){
          case 0: //add operation
                  printf("\tClient%d: Sending add request to server\n",id);
                  messageString = "amsg";
                  length = strlen(messageString);
                  numMessages = length/10 + (length%10 !=0);
                  msg[2]=numMessages;
                  //Message Header
                  Send(&p[serverPort],msg);
                  messages = encode(messageString);

                  // printf("Printing Matrix on client side\n");
                  // print_matrix(numMessages,messages);
                  // printf("%s\n", decode(messages,numMessages));
                  
                  for(i=0;i<numMessages;i++){
                    for(j=0; j<10; j++)
                      msg[j]=messages[i][j];
                    //print_matrix(1,msg);
                    Send(&p[serverPort],msg);
                  }
                  free_matrix(numMessages,messages);
                  Receive(&p[id],msg);
                  if(msg[0])
                    printf("\tClient%d: Add request for message \"%s\" acknowledged by Server\n", id,messageString);
                  else
                    printf("\tClient%d: Add request for message \"%s\" not acknowledged by Server\n", id,messageString);
                  break;
          case 1: //delete  operation
                  printf("\tClient%d: Sending delete request to server\n",id);
                  //Message Header
                  Send(&p[serverPort],msg);
                  Receive(&p[id],msg);
                  if(msg[0])
                    printf("\tClient%d: Delete request acknowledged by Server\n", id);
                  else
                    printf("\tClient%d: Delete request not acknowledged by Server\n", id);
                  break;
          case 2: //modify operation
                  printf("\tClient%d: Sending modify request to server\n",id);
                  messageString = "mmsg";
                  length = strlen(messageString);
                  numMessages = length/10 + (length%10 !=0);
                  msg[2]=numMessages;
                  msg[3]=rand()%10;
                  printf("\tClient%d: Modification location is %d\n",id, msg[3]);
                  //Message Header
                  Send(&p[serverPort],msg);
                  messages = encode(messageString);

                  // printf("Printing Matrix on client side\n");
                  // print_matrix(numMessages,messages);
                  // printf("%s\n", decode(messages,numMessages));
                  
                  for(i=0;i<numMessages;i++){
                    for(j=0; j<10; j++)
                      msg[j]=messages[i][j];
                    //print_matrix(1,msg);
                    Send(&p[serverPort],msg);
                  }
                  free_matrix(numMessages,messages);
                  Receive(&p[id],msg);
                  if(msg[0])
                    printf("\tClient%d: Modify request for message \"%s\" acknowledged by Server\n", id,messageString);
                  else
                    printf("\tClient%d: Modify request for message \"%s\" not acknowledged by Server\n", id,messageString);
                  break;      
        }
        sleep(1);
    }
    
}

//send request to server
//server sends a message array with the number of messages per table entry
void displayClient(void){
    //No mutex needed as we are using user threads
    

    int id=clientPorts++;
    char * table[10];
    int **messages=0;
    char * messageString=0;
    int msg[10]={0},i,j,k;
    int numMsgs[10] = {0};
    int serverPort;
    int location=0;
    int length=0,numMessages=0;
    for(i=0;i<10;i++)
      table[i]=NULL;
    while(1){
        
        serverPort = srvPort;
        // currentServerPort = (currentServerPort+1) % numServerPorts;
        int operation = 3;
        //int operation = 0;
        //Sending the client port on the array index 0 of the message.
        msg[0]=id;
        msg[1]=operation;
        

        switch(operation){
         
          case 3: //display operation
                  printf("\t\tClient%d: Sending display request to server\n",id);
                  Send(&p[serverPort],msg);

                  //Receive message header from server which will contain the number of entries for each message
                  Receive(&p[id],msg);

                  for(i=0;i<10;i++)
                    numMsgs[i]=msg[i];

                  for(i=0;i<10;i++){

                    if(numMsgs[i]!=-1){
                      messages = (int **) malloc(sizeof(int *)*numMessages);
                      for(j=0;j<numMsgs[i];j++){
                        messages[j] = (int *) malloc(sizeof(int)*10); 
                      }

                      //Reconstruct message array
                      for(j=0;j<numMsgs[i];j++){
                        Receive(&p[id],msg);
                        //printf("Server%d: Received add message%d from Client%d\n",id,i,replyPort);
                        //print_matrix(1,msg);
                        for(k=0;k<10;k++)
                          messages[j][k] = msg[k];
                      }
                    // printf("Printing Matrix on server side\n");
                    // print_matrix(numMessages,messages);
                      //printf("\t\tTable Entry[%d]: %s\n", i, );
                      table[i]=decode(messages,numMsgs[i]);
                      free_matrix(numMsgs[i],messages);
                    }
                  }

                  for(i=0;i<10;i++){
                    if(table[i]==NULL)
                      printf("\t\tTable Entry[%d]: Empty\n", i);
                    else
                      printf("\t\tTable Entry[%d]: %s\n", i, table[i]);
                  }

                  for(i=0;i<10;i++){
                    if(table[i]!=NULL)
                      free(table[i]);
                  }
                  break;
          
        }
        sleep(1);
    }
    
}

//-------------------------------------------------------

int main()
{
    srand(time(NULL));
    init_ports();
   // int i;
    
    //Creating 5 servers.
    //for(i=0;i<5;i++)
        start_thread(server);

    //Creating 90 clients.
    //for(i=10;i<100;i++)
        start_thread(client);
        start_thread(client);
        start_thread(displayClient);
    run();
    while (1) sleep(1);
}   

// int main(){
//   srand(time(NULL));
//   int **matrix;
//   /* First matrix */
//   // printf("Matrix 1\n");
//   // printf("Enter # of rows and cols: ");
//   // scanf("%d",&rows);
//   int length = strlen("HeyWhatsupY");
//   int rows = length/10 + (length%10 !=0);
  
//   matrix = encode("HeyWhatsupY"); 
//   printf("Your Matrix\n");  /* Print the entered data */
//   print_matrix(rows,matrix);
  
//   char * key;
//   key = (char*)malloc(3 * sizeof(char));
//   key[0] = 'a';
//   key[1] = 'a';
//   key[2] = '\0';
//   printf("%s\n", key);
//   free(key);
//   char * decodedString = decode(matrix,rows);
//   printf("%s\n", decodedString);
//   free_matrix(rows, matrix);   /* Free the matrix */
//   free(decodedString);
//   return 0;
//  }
