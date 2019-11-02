/*************************************************************
*
  Author: Denny Abraham Cheriyan, Adrin Peter Fernandes
  Testing threads.h using infinite functions

*/

#include <thread.h>
#include <unistd.h> // sleep
#include <signal.h> // SIGCHLD
#include <sched.h> // clone
#include <sys/wait.h> // wait
#include <thread> // std::this_thread
#include <chrono> // std::chrono_literals

#include <stack.h>
#include <timer.h>

using namespace std::chrono_literals;

void thr1() {
	while(1) {
        printf("thr1 sleep for 2 seconds!\n");
        // sleep(2) resumes on receiving a caught signal
		std::this_thread::sleep_for(2s);
		printf("slept!\n");
	}
}

void thr2() {
	while(1) {
		printf("thr2 sleep for 2 seconds!\n");
		// sleep(2) resumes on receiving a caught signal
		std::this_thread::sleep_for(2s);
		printf("slept!\n");
	}
}

bool firstrun = false;

Timer timer = Timer();

void Timer::timer_handler(int signum) {
	static int count = 1;
	if (!firstrun) {
		fprintf (stderr, "> FIRST RUN timer ms = %d, count = %03d\n", timer.ms_, count++);
		firstrun = true;
		run();
	} else {
		fprintf (stderr, "> YIELDING timer ms = %d, count = %03d\n", timer.ms_, count++);
		yield();
	}
}

int YIELD_THREAD(void * arg) {
	// timer is per process, the process it is set in must exist in order to recieve a signal
	timer.set_timer(timer.Types.ITIMER_REAL, Timer::timer_handler, 1000, false);
	pause();
	return 0;
}

int main(){
	/* Flush each printf() as it happens. */
	setvbuf(stdout, 0, _IOLBF, 0);
	setvbuf(stderr, 0, _IOLBF, 0);

	start_thread(thr1);
	start_thread(thr2);

	Stack x = Stack();
	x.alloc(4096);
	// start in a new process to prevent its signal handler from being modified once set
	long int pid = clone(YIELD_THREAD, x.top, CLONE_VM|SIGCHLD, nullptr);
	if (pid == -1) perror("YIELD_THREAD clone");
	// sleep for 1 second to allow for pause
	sleep(1);
	// a stack is not needed since the process pauses
	x.free();
	// let threads to do something for 8 seconds
	sleep(8);
	puts("exiting");
	kill(pid, SIGTERM);
	return 0;
}
