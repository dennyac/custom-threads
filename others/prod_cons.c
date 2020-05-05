// (c) Partha Dasgupta 2009
// permission to use and distribute granted.
// We can use two different mutexes here right??

#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

#define N 4
Semaphore_t empty, full, mutex;
int buffer[N];
int in = 0, out = 0, item_num=0, prod_delay = 1, cons_delay = 1, global_ID=0;


void prod()
{
    int ID = global_ID++;
    while (1){ 
	printf("Producer %d: ready to produce\n", ID);
        P(&empty);
          P(&mutex);
	    printf("Producer %d: inserting item#%d, into slot #%d\n", ID, item_num, in);
            buffer[in] = item_num++; in = (in+1) % N;
          V(&mutex);
	V(&full);
	sleep(prod_delay);
    }
}    

void cons()
{
    int ID = global_ID++;
    while(1){
	printf("    Consumer %d: ready to consume\n", ID);
        P(&full);
          P(&mutex);
	    printf("   Consumer %d: deleting item#%d, from slot #%d\n", ID, buffer[out], out);
            out = (out+1) % N;
          V(&mutex);
	V(&empty);
        sleep(cons_delay);
    }    
}


int main()
{
    int id[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int count = 0;
    full = CreateSem(0);
    empty = CreateSem(N);
    mutex = CreateSem(1);

    start_thread(prod);
    start_thread(cons);
    start_thread(prod);
    start_thread(cons);
    start_thread(prod);
    start_thread(cons);
    start_thread(prod);
    start_thread(cons);
    run();
    while (1) { scanf("%d %d", &prod_delay, &cons_delay); 
                printf ("\n\n\t\t\t\tP=%d C=%d\n\n\n", prod_delay, cons_delay);
    };
}




