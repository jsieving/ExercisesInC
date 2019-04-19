/* Example code for Exercises in C.

Copyright 2016 Allen B. Downey
License: MIT License https://opensource.org/licenses/MIT

*/

/*
On my machine, forking a process gives it its own virtual address space and
copies global data from the parent's adress space to the child's on creation.

The parent process usually (but not necessarily!) prints first, so I used it to
modify data stored at predetermined, global locations to check if these changes
affected its children. The memory addresses printed as being the same, but had
different changes.

Global data can be shared at first, but if it is modified by one process, the
other processes will not see the change.

Heap data is not shared. When I assigned a string literal to a malloc'd pointer
in the parent process, that same pointer pointed to NULL for the child following
it.

Stack data is not shared. One piece of evidence for this is that an arbitrary
address on the stack is very close between the parent and child (40B) and equal
between children. This implies that the stack virtual address space is not
partitioned and shared when forking, rather, each process has its own virtual
address space. (I think the 40B difference is because the child and parent are
actually at different points in the code execution when they print.)

The other evidence is that if I try to read from a parent's stack address in the
child, the child's part of the code just won't print - I suspect a segfault. :)

They appear to share a code segment, but I suspect it's just copied over when
fork is executed, like the global segment.

Relevant: lines 55-60, 75-85, 109-115, 138-151,
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <wait.h>


// errno is an external global variable that contains
// error information
extern int errno;
// each child gets a copy of the globals space.
// Changing the parent doesn't translate to the child.
char* global_word = "Global";
// which is why I will have to initialize this in main(), and not just
// in the parent - the child would not have the actual address in its copy.
char** heap_pointer;
// we're going to try to share a stack address, and guess what, it won't work!
char** stack_pointer;

// get_seconds returns the number of seconds since the
// beginning of the day, with microsecond precision
double get_seconds() {
    struct timeval tv[1];

    gettimeofday(tv, NULL);
    return tv->tv_sec + tv->tv_usec / 1e6;
}


void child_code(int i)
{
    sleep(i);
    /* This will have an address close to the parent's version, implying that
    the stack space has NOT been partitioned to share with the child. */
    char* word =  "Child stack";
    printf("Hello from child %d.\n\
    Address of %s is %p\n\
    Address of %s is %p\n\
    Address of %s is %p\n\
    Address of get_seconds() is %p\n",
    i, global_word, &global_word, *heap_pointer, heap_pointer, word, &word, get_seconds);
    // Replacing with this causes a segfault! -> *stack_pointer, stack_pointer);
    // That implies that the child is trying to reach into the parent's address space.
}

// main takes two parameters: argc is the number of command-line
// arguments; argv is an array of strings containing the command
// line arguments
int main(int argc, char *argv[])
{
    int status;
    pid_t pid;
    double start, stop;
    int i, num_children;

    // the first command-line argument is the name of the executable.
    // if there is a second, it is the number of children to create.
    if (argc == 2) {
        num_children = atoi(argv[1]);
    } else {
        num_children = 1;
    }

    // get the start time
    start = get_seconds();

    /* get an address on the heap - I can't use malloc outside of main() because
    the result of malloc is not constant. However, heap_pointer is initialized
    outside of main(), so this will be a global value anyway.
    Importantly, it is written in globals BEFORE the children are forked.
    Otherwise, the children would get a COPY of heap_pointer BEFORE it was
    initialized. */
    heap_pointer = malloc(sizeof(int));

    for (i=0; i<num_children; i++) {

        // create a child process
        printf("Creating child %d.\n", i);
        pid = fork();

        /* check for an error */
        if (pid == -1) {
            fprintf(stderr, "fork failed: %s\n", strerror(errno));
            perror(argv[0]);
            exit(1);
        }

        /* see if we're the parent or the child */
        if (pid == 0) {
            child_code(i);
            exit(i);
        }
    }

    /* parent continues */
    // modify a global value, AFTER children were created.
    global_word = "parent waz here";
    // set a globally-known memory location in the heap, AFTER children were created.
    *heap_pointer = "Parent heap";
    // make a char* in the stack...
    char* word =  "Parent stack";
    // ...and hand its address to this global variable.
    stack_pointer = &word;
    printf("Hello from the parent.\n\
    Address of %s is %p\n\
    Address of %s is %p\n\
    Address of %s is %p\n\
    Address of get_seconds() is %p\n",
    global_word, &global_word, *heap_pointer, heap_pointer, *stack_pointer, stack_pointer, get_seconds);

    for (i=0; i<num_children; i++) {
        pid = wait(&status);

        if (pid == -1) {
            fprintf(stderr, "wait failed: %s\n", strerror(errno));
            perror(argv[0]);
            exit(1);
        }

        // check the exit status of the child
        status = WEXITSTATUS(status);
        printf("Child %d exited with error code %d.\n", pid, status);
    }
    // compute the elapsed time
    stop = get_seconds();
    printf("Elapsed time = %f seconds.\n", stop - start);

    exit(0);
}
