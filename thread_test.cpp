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

#include <thread> // std::this_thread
#include <chrono> // std::chrono_literals

using namespace std::chrono_literals;

void thr1() {
	while(1) {
        printf("thr1 sleep for 2 seconds!\n");
        // sleep(2) resumes on recieving a caught signal
		std::this_thread::sleep_for(2s);
		printf("slept!\n");
	}
}

void thr2() {
	while(1) {
		printf("thr2 sleep for 2 seconds!\n");
		// sleep(2) resumes on recieving a caught signal
		std::this_thread::sleep_for(2s);
		printf("slept!\n");
	}
}

#include <stdlib.h>   // exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <signal.h>   // sigaction()
#include <stdio.h>    // printf(), fprintf(), stdout, stderr, perror(), _IOLBF
#include <string.h>   // memset()
#include <sys/time.h> // ITIMER_REAL, ITIMER_VIRTUAL, ITIMER_PROF, struct itimerval, setitimer()
#include <stdbool.h>  // true, false
#include <limits.h>   // INT_MAX
#include <cassert>

class Timer {
		public:
		int type_ = 0;
		void (*handler_) (int) = nullptr;
		int ms_ = 0;
		bool oneShot_ = true;
		class Types {
			public:
			enum {
				ITIMER_REAL,
				ITIMER_VIRTUAL,
				ITIMER_PROF
			};
		} Types;
		int timer_signal(int timer_type);
		void set_timer(int type, void (*handler) (int), int ms, bool oneShot);
		static void timer_handler (int signum);
};

int Timer::timer_signal(int timer_type) {
	int sig;

	switch (timer_type) {
		case ITIMER_REAL:
			sig = SIGALRM;
			break;
		case ITIMER_VIRTUAL:
			sig = SIGVTALRM;
			break;
		case ITIMER_PROF:
			sig = SIGPROF;
			break;
		default:
			fprintf(stderr, "ERROR: unknown timer type %d!\n", timer_type);
			exit(EXIT_FAILURE);
	}

	return sig;
}

void Timer::set_timer(int type, void (*handler)(int), int ms, bool oneShot) {
	type_ = type;
	handler_ = handler;
	ms_ = ms;
	oneShot_ = oneShot;
	printf("oneshot: %s\n", oneShot ? "true" : "false");
	struct itimerval timer;
	struct sigaction sa;

	/* Install signal handler for the timer. */
	memset (&sa, 0, sizeof (sa));
	sa.sa_handler =  handler;
	sigaction (timer_signal(type), &sa, NULL);

	/* Configure the timer to expire after ms msec... */
	// convert ms to seconds and ms

	int milliseconds = ms;
	int seconds = milliseconds;
	if (seconds > 999) {
		seconds /= 1000;
		milliseconds -= seconds * 1000;
	} else seconds = 0;
	printf("ms = %d\nmilliseconds = %d\nseconds = %d\n", ms, milliseconds, seconds);

	assert(seconds <= 60);

	timer.it_value.tv_sec = seconds;
	timer.it_value.tv_usec = milliseconds;

	if (oneShot) {
		timer.it_interval.tv_sec = 0;
		timer.it_interval.tv_usec = 0;
	} else {
		timer.it_interval.tv_sec = seconds;
		timer.it_interval.tv_usec = milliseconds;
	}
	if (setitimer (type, &timer, NULL) < 0) {
		perror("Setting timer");
		exit(EXIT_FAILURE);
	};
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

int THREAD_CREATE(void * nu) {
	Stack x = Stack();
	x.alloc(4096);
	puts("THREAD_CREATE starting YIELD_THREAD");
	long int pid = clone(YIELD_THREAD, x.top, CLONE_VM|SIGCHLD, nullptr);
	if (pid == -1) perror("THREAD_CREATE clone");
	// sleep for 1 second to allow for pause
	sleep(1);
	// a stack is not needed since the process pauses
	x.free();
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
	puts("starting THREAD_CREATE");
	long int pid = clone(THREAD_CREATE, x.top, CLONE_VM|SIGCHLD, nullptr);
	if (pid == -1) {
		perror("clone");
		x.free();
		return 0;
	}
	puts("waiting for THREAD_CREATE");
	if (waitid(P_PID, pid, nullptr, WEXITED) == -1) {
		perror("waitid");
		x.free();
		return 0;
	}
	puts("waited for THREAD_CREATE");
	x.free();
	pause();
	return 0;
}