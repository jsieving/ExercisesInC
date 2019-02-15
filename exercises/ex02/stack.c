/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

*/

#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

int* foo() {
  // returns a pointer to {42, 42, 42, 42, 42}
    int i;
    int array0[SIZE];

    printf("foo() array0 address: %p\n", array0); // still segfaults if I comment these

    for (i=0; i<SIZE; i++) {
        array0[i] = 42;
    }
    return array0;
    // this is local to foo(), disappears when foo() finishes (address becomes meaningless)
}

void bar() {
  /* creates an array of {0, 1, 2, 3, 4}, but no reference to
     the array is saved by the program since this returns null and doesn't
     affect a global variable */
    int i;
    int array1[8]; // created this to investigate below
    int array2[SIZE]; // Wait, why does this have the same address as foo's array???

    /* if array1 has size 5 or less, then array2 seems to have the same address as the
    array in foo(). If it's larger, then array2 has a different address.

    Wait, now if I print the address of array1, it starts below where foo() started and
    builds upward... Seems like:

    if size(array1) = 4, array1 gets 16 B and is allocated below array2
    or if size(array1) = 5, it gets 32 B and is allocated below
        array2 ends up in the same location as old array0
    if size(array1) = 6, it's located where array0 was, and array2 goes below it
    if size(array1) = 7-8, it starts 16 B lower than array0 did, and array2 goes below

    Okay, this is interesting but it seems like there's little point in exploring
    every configuration. :)
    */

    printf("bar() array1 address: %p\n", array1); // still segfaults if I comment these
    printf("bar() array2 address: %p\n", array2); // what was supposed to change?

    for (i=0; i<SIZE; i++) {
        array1[i] = i;
    }
}

int* fooTwo() {
  // returns a pointer to {42, 42, 42, 42, 42}
    int i;
    static int array[SIZE]; // "static" tosses it into globals

    printf("fooTwo() array address: %p\n", array); // much lower address, with globals.

    for (i=0; i<SIZE; i++) {
        array[i] = 42;
    }
    return array;
}

int main()
{
    int i;
    int *array = fooTwo();
    bar();

    for (i=0; i<SIZE; i++) {
        printf("%d\n", array[i]);
    }

    return 0;
}
