/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

----------------------------------
counter.c

2) See uploaded thread diagram.
  The final value of the counter is correct, because it is printed after the
  parent waits for all the children, so each child has had a chance to increment
  it once.

3) I can tell the threads are running concurrently because there is no
  predicting how many of the threads will increment the counter before others
  print it. They could each print 0 and then each increment it, or they could
  each print a number and increment, in the "intended" order, or any arrangement
  as long as each individual thread prints before it increments.
  Synchronization errors occur, in the sense that no effort has been made to
  syncronize the threads at all.

counter_array.c

2) Yes, synchronization errors occur: the checker says that a lot (>100,000) of
  entries have incorrect values.
  Also, sometimes when this runs, the child code incorrectly prints the shared
  counter, which makes it seem like one thread is incrementing it after the
  % 10000 == 0 check, but before it actually prints.

3) Done, no errors! The mutex lock/unlock doesn't really need to surround the
  print statement to avoid errors, but the printed output will be correct if the
  mutex covers it.

4) Here's the output from running both programs:
-----------------------------------
$ time ./counter_array
  Starting child at counter 0
  Starting child at counter 9788
  Child done.
  Child done.
  Checking...
  135413 errors.

  real	0m0.049s
  user	0m0.080s
  sys	0m0.008s

$ time ./counter_array_mutex
  Starting child at counter 0
  Starting child at counter 0
  Child done.
  Child done.
  Checking...
  0 errors.

  real	0m0.139s
  user	0m0.142s
  sys	0m0.126s
-----------------------------------

Synchronization ~triples the start-to-finish time of the program (real).
It ~doubles the amount of time in user mode (running the process outside the kernel).
It vastly increases the amount of time in sys mode (system calls within the kernel).
  Maybe this is because the mutex has to alert the kernel when it's been unlocked,
  so the other thread can run, but I thought that threads and their locks were
  handled within the process in user mode, not by the kernel?

Note: I think the 'user' time can exceed the 'real' time because 'user' time is
the sum of 'user' time for all threads.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mutex.h"

#define NUM_CHILDREN 2

void perror_exit(char *s)
{
    perror(s);
    exit(-1);
}

void *check_malloc(int size)
{
    void *p = malloc(size);
    if (p == NULL) {
        perror_exit("malloc failed");
    }
    return p;
}

typedef struct {
    int counter;
    int end;
    int *array;
    Mutex* mutex;
} Shared;

Shared *make_shared(int end)
{
    int i;
    Shared *shared = check_malloc(sizeof(Shared));

    shared->counter = 0;
    shared->end = end;
    shared->mutex = make_mutex();

    shared->array = check_malloc(shared->end * sizeof(int));
    for (i=0; i<shared->end; i++) {
        shared->array[i] = 0;
    }
    return shared;
}

pthread_t make_thread(void *(*entry)(void *), Shared *shared)
{
    int ret;
    pthread_t thread;

    ret = pthread_create(&thread, NULL, entry, (void *) shared);
    if (ret != 0) {
        perror_exit("pthread_create failed");
    }
    return thread;
}

void join_thread(pthread_t thread)
{
    int ret = pthread_join(thread, NULL);
    if (ret == -1) {
        perror_exit("pthread_join failed");
    }
}

void child_code(Shared *shared)
{
    printf("Starting child at counter %d\n", shared->counter);

    while (1) {
        if (shared->counter >= shared->end) {
            return;
        }
        mutex_lock(shared->mutex);
        shared->array[shared->counter]++;
        shared->counter++;
        // if (shared->counter % 10000 == 0) {
        //     printf("%d\n", shared->counter);
        // }
        mutex_unlock(shared->mutex);
    }
}

void *entry(void *arg)
{
    Shared *shared = (Shared *) arg;
    child_code(shared);
    printf("Child done.\n");
    pthread_exit(NULL);
}

void check_array(Shared *shared)
{
    int i, errors=0;

    printf("Checking...\n");

    for (i=0; i<shared->end; i++) {
        if (shared->array[i] != 1) errors++;
    }
    printf("%d errors.\n", errors);
}

int main()
{
    int i;
    pthread_t child[NUM_CHILDREN];

    Shared *shared = make_shared(1000000);

    for (i=0; i<NUM_CHILDREN; i++) {
        child[i] = make_thread(entry, shared);
    }

    for (i=0; i<NUM_CHILDREN; i++) {
        join_thread(child[i]);
    }

    check_array(shared);
    return 0;
}
