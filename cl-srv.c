#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msgs.h"

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




//Allocating ports 0 to 9 for servers
int numServerPorts = 0;
int currentServerPort = 0;

//Allocating ports 10 to 99 for clients
int clientPorts = 10;


void server(void){
    //No mutex needed as we are using user threads
    char * table[10];
    int pos=0;
    int id=numServerPorts++;
    int msg[10],i,j;
    while(1){      

        //printf("Server %d: Receiving message on port %d\n",id,id);
        Receive(&p[id],msg);
        printf("Server %d: Received message on port %d\n",id,id);

        switch(msg[1]){
          case 0: //add operation
                  printf("Server%d: Received add message from Client%d\n",id,msg[0]);
                  int replyPort = msg[0];
                  int numMessages = msg[2];

                  int **messages = (int **) malloc(sizeof(int *)*numMessages);
                  for(i=0; i<numMessages; i++)
                  /* Allocate array, store pointer  */
                      messages[i] = (int *) malloc(sizeof(int)*10); 

                  //Reconstruct message array
                  for(i=0;i<numMessages;i++){
                    Receive(&p[id],msg);
                    printf("Server%d: Received add message%d from Client%d\n",id,i,msg[0]);
                    //print_matrix(1,msg);
                    for(j=0;j<10;j++)
                      messages[i][j] = msg[j];
                  }
                  printf("Printing Matrix on server side\n");
                  print_matrix(numMessages,messages);
                  printf("Decoded message at server%s\n", decode(messages,numMessages));
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
        }
        for(i=0;i<pos;i++)
          printf("%s\n", table[i]);
        sleep(1);
    }   
}

void client(void){
    //No mutex needed as we are using user threads
    

    int id=clientPorts++;
    int msg[10]={0},i,j;
    int serverPort;
    while(1){
        
        serverPort = currentServerPort;
        currentServerPort = (currentServerPort+1) % numServerPorts;
        //int operation = rand()%4;
        int operation = 0;
        //Sending the client port on the array index 0 of the message.
        msg[0]=id;
        msg[1]=operation;
        

        switch(operation){
          case 0: //add operation
                  printf("Client%d: Sending add message to server\n",id);
                  char * messageString = "MessageMessage";
                  int length = strlen(messageString);
                  int numMessages = length/10 + (length%10 !=0);
                  msg[2]=numMessages;
                  //Message Header
                  Send(&p[serverPort],msg);
                  int **messages = encode(messageString);

                  printf("Printing Matrix on client side\n");
                  print_matrix(numMessages,messages);
                  printf("%s\n", decode(messages,numMessages));
                  
                  for(i=0;i<numMessages;i++){
                    for(j=0; j<10; j++)
                      msg[j]=messages[i][j];
                    printf("Client%d: Sending add message%d to server\n",id,i);
                    //print_matrix(1,msg);
                    Send(&p[serverPort],msg);
                  }
                  free_matrix(numMessages,messages);
                  Receive(&p[id],msg);
                  if(msg[0])
                    printf("Client%d: Message %s acknowledged by Server\n", id,messageString);
                  else
                    printf("Client%d: Message %s not acknowledged by Server\n", id,messageString);
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
