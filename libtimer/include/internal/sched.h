//
// Created by brothercomplex on 2/11/19.
//

#ifndef custom_threads_libtimer_sched
#define custom_threads_libtimer_sched

#include <stack.h>
#include <unistd.h> // sleep
#include <signal.h> // SIGCHLD
#include <sched.h> // clone
#include <sys/wait.h> // wait

class Sched {
    long int pid;
    public:
        static int yield(void * arg);
        Stack x = Stack();
        Sched() {
            puts("starting Scheduler");
            x.alloc(4096);
            // start in a new process to prevent its signal handler from being modified once set
            pid = clone(yield, x.top, CLONE_VM|SIGCHLD, nullptr);
            if (pid == -1) perror("YIELD_THREAD clone");
            puts("successfully started Scheduler");
        }
        ~Sched() {
            if (pid != -1) {
                puts("stopping Scheduler");
                kill(pid, SIGTERM);
                siginfo_t info;
                if (waitid(P_PID, pid, &info, WEXITED | WSTOPPED) == -1) {
                    perror("waitid");
                    puts("failed to stop Scheduler");
                    return;
                }
                if (info.si_code == CLD_KILLED && info.si_status == SIGTERM)
                    puts("successfully stopped Scheduler");
                else puts("failed to stop Scheduler");
                x.free();
            }
        }
};

#endif //custom_threads_libtimer_sched
