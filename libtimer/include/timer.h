//
// Created by brothercomplex on 2/11/19.
//

#ifndef custom_threads_libtimer
#define FOREGROUND_SERVICE_TIMER_H


#include <stdlib.h>   // exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <signal.h>   // sigaction()
#include <stdio.h>    // printf(), fprintf(), stdout, stderr, perror(), _IOLBF
#include <string.h>   // memset()
#include <sys/time.h> // ITIMER_REAL, ITIMER_VIRTUAL, ITIMER_PROF, struct itimerval, setitimer()
#include <stdbool.h>  // true, false
#include <limits.h>   // INT_MAX

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

#endif //FOREGROUND_SERVICE_TIMER_H
