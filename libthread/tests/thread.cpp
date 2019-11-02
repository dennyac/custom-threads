/*************************************************************
*
  Author: Denny Abraham Cheriyan, Adrin Peter Fernandes
  Testing threads.h using infinite functions

*/

#include <thread.h>
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

// declare a custom timer handler

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

// declare a custom schedule yeilder

int Sched::yield(void * arg) {
    // timer is per process, the process it is set in must exist in order to recieve a signal
    timer.set_timer(timer.Types.ITIMER_REAL, Timer::timer_handler, 1000, false);
    pause();
    return 0;
}

int main(){
	/* Flush each printf() as it happens. */
	setvbuf(stdout, 0, _IOLBF, 0);
	setvbuf(stderr, 0, _IOLBF, 0);

	Sched foo = Sched(); // scoped initialization and de-initialization of the preemptive scheduler

    start_thread(thr1);
    start_thread(thr2);

	// let threads to do something for 8 seconds
	sleep(8);
	puts("exiting");
	return 0;
}
