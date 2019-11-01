/*************************************************************
*
  Author: Denny Abraham Cheriyan, Adrin Peter Fernandes
  Testing threads.h using infinite functions

*/

#include "threads.h"
#include <unistd.h> // sleep
#include <signal.h> // SIGCHLD
#include <sched.h> // clone
#include <sys/wait.h> // wait
#include "stack/stack.h" // Stack
#include "stack/direction.cpp" // Stack
#include "stack/new.cpp" // Stack

void thr1() {
	while(1) {
        printf("1 Hello World!\n");
        fflush(stdout);
        sleep(1);
	}
}

void thr2() {
	while(1) {
        printf("2 Hello World!\n");
        fflush(stdout);
        sleep(2);
	}
}

bool firstrun = false;
int YIELD_THREADX(void * arg) {
    if (!firstrun) {
        puts("FIRST RUN");
        firstrun = true;
        run();
    } else {
        puts("YIELDING");
        yield();
    }
	return 0;
}

int YIELD_THREAD(void * arg) {
	while(1) {
		Stack x = Stack();
		x.alloc(4096);
		long int pid = clone(YIELD_THREADX, x.top, CLONE_VM|SIGCHLD, nullptr);
		if (pid == -1) {
			perror("THREAD_CREATE clone");
		}
        sleep(5);
	}
	return 0;
}

int THREAD_CREATE(void * nu) {
	start_thread(thr1);
	start_thread(thr2);
	Stack x = Stack();
	x.alloc(4096);
	puts("THREAD_CREATE starting YIELD_THREAD");
	long int pid = clone(YIELD_THREAD, x.top, CLONE_VM|SIGCHLD, nullptr);
	if (pid == -1) {
		perror("THREAD_CREATE clone");
		x.free();
	}
	puts("THREAD_CREATE running threads");
	return 0;
}

int main(){
	Stack x = Stack();
	x.alloc(4096);
	puts("starting THREAD_CREATE");
	long int pid = clone(THREAD_CREATE, x.top, CLONE_VM|SIGCHLD, nullptr);
	if (pid == -1) {
		perror("clone");
		x.free();
	}
	puts("waiting for THREAD_CREATE");
	if (waitid(P_PID, pid, nullptr, WEXITED) == -1) {
		perror("waitid");
		x.free();
		return 0;
	}
	puts("waited for THREAD_CREATE");
	puts("sleeping for 10 seconds");
	sleep(10);
	puts("slept for 10 seconds");
	x.free();
	return 0;
}