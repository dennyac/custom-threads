#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(){

  int **matrix;
  /* First matrix */
  // printf("Matrix 1\n");
  // printf("Enter # of rows and cols: ");
  // scanf("%d",&rows);
  int length = strlen("HeyWhatsupY");
  int rows = length/10 + (length%10 !=0);
  
  matrix = encode("HeyWhatsupY"); 
  printf("Your Matrix\n");  /* Print the entered data */
  print_matrix(rows,matrix);
  
  char * key;
  key = (char*)malloc(3 * sizeof(char));
  key[0] = 'a';
  key[1] = 'a';
  key[2] = '\0';
  printf("%s\n", key);
  free(key);
  char * decodedString = decode(matrix,rows);
  printf("%s\n", decodedString);
  free_matrix(rows, matrix);   /* Free the matrix */
  free(decodedString);
  return 0;
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