/*************************************************************
*
  Author: Denny Abraham Cheriyan, Adrin Peter Fernandes
  Testing threads.h using infinite functions

*/

#include "threads.h"

void func1(){
	int i=0;
	while(1){
		printf("Executing within func1\n");
		i+=1;
		yield();
		printf("Func1 Value - %d\n", i);
	}
}

void func2(){
	int i=0;
	while(1){
		printf("Executing within func2\n");
		i+=2;
		yield();
		printf("Func2 Value - %d\n", i);
	}
}

void func3(){
	int i=0;
	while(1){
		printf("Executing within func3\n");
		i+=3;
		yield();
		printf("Func3 Value - %d\n", i);
	}
}

int main(){
	start_thread(func1);
	start_thread(func2);
	start_thread(func3);
	run();
	return 0;
}