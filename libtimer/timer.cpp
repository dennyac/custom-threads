//
// Created by brothercomplex on 2/11/19.
//

#include "include/timer.h"

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

    if (seconds > 60) {
        puts("error: seconds must not be greater than 60 seconds");
        abort();
    }

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