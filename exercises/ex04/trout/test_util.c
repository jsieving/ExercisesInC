/* Example code for Software Systems at Olin College.

Copyright 2017 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "minunit.h"

int tests_run = 0;

static char *test1() {
    void* res = Calloc(32, sizeof(int));
    char *message = "test1 failed: Calloc should return non-null pointer";
    mu_assert(message, res != NULL);
    return NULL;
}

static char *test2() {
    int* res = Calloc(32, sizeof(int));
    int empty = 1;
    for (int i = 0; i < 32; i++) {
      if (res[i] != 0) {
        empty = 0;
        break;
      }
    }
    char *message2 = "test2 failed: Calloc should set returned memory to NULL";
    mu_assert(message2, empty == 1);
    return NULL;
}

static char *test3() {
    char buf[] = "This is a test.";
    char* readbuf = malloc(32);
    int f = open("testwrite.txt", O_RDWR|O_CREAT|O_TRUNC);
    Write(f, buf, sizeof(buf));
    lseek(f, 0, SEEK_SET);
    ssize_t res = Read(f, readbuf, sizeof(buf));
    char *message = "test3 failed: 16 bytes should have been written and read";
    mu_assert(message, res == 16);
    return NULL;
}

static char * all_tests() {
    mu_run_test(test1);
    mu_run_test(test2);
    mu_run_test(test3);
    return NULL;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != NULL) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
