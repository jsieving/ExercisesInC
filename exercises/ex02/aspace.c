/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

*/

#include <stdio.h>
#include <stdlib.h>

int var1; // global: globals

void printSomething() {
  int a = 24;
  printf("Address of a is %p\n", &a);
}

int main () // program instruction: code text
{
    int var2 = 5; // local: stack
    void *p = malloc(128); // dynamically allocated memory: heap
    void *q = malloc(64);
    void *r = malloc(32);
    void *t = malloc(16);
    void *u = malloc(1);
    void *v = malloc(1);
    char *s = "Hello, World"; // string literal: constants

    printf ("Address of main is %p\n", main); // lowest
    printf ("Address of var1 is %p\n", &var1); // middle
    printf ("Address of var2 is %p\n", &var2); // highest
    printf ("p points to %p\n", p); // second highest (spaced 144)
    printf ("q points to %p\n", q); // heap grows upward (spaced 80)
    printf ("r points to %p\n", r); // spaced 48
    printf ("t points to %p\n", t); // spaced 32
    printf ("u points to %p\n", u); // spaced 32
    printf ("u points to %p\n", v); //
    printf ("s points to %p\n", s); // second lowest
    printSomething(); // stack grows down (Lol, Atom tried to autocomplete with "Downey")

    return 0;
}

/*
Difference between text address and constants is very small, I'm guessing one of them has very little allocated space. Globals is a bigger leap up.

The heap starts pretty low, close to the other memory segments. Makes sense.

Locations (highest to lowest addresses):

stack
|
|
|
...
|
|
|
heap
|
|
globals
|
constants
code text

*/
