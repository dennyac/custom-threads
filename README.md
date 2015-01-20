# README #

Routines that will allow a program to run, with multiple threads, using NON-preemptive scheduling on most Unix/Linux operating systems. Contains implementation of custom semaphores, and sample programs to test threads and semaphores.

### Requirements ###

* Ubuntu 14.04
* gcc compiler

### To test threads library ###

1. gcc thread_test.c
2. ./a.out | head -40