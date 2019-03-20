/*  Test code for the functions in rand.c

Copyright 2016 Allen B. Downey
License: MIT License https://opensource.org/licenses/MIT
*/

/* RESPONSES:

Question 4: the fastest random float generator (which serves the intended purpose and isn't a dummy function is random_float, approximately tied with my_random_float. random_float uses the standard algorithm, while my_random_float uses Allen's algorithm explained in his paper. Some of the operations are done in embedded assembly to speed it up compared to the paper's implementation. As a result, it works almost as fast as the standard, sometimes beating it in the time test.

I'd like to point out that the uniformity graph has no way of showing the advantage that Allen's method claims to have over the standard.

Question 6: random_double is about twice as fast as my_random_double. I think it would have been a lot closer if I had optimized it like Allen did with the faster my_random_float. Interestingly, my_random_double takes about as long as my_random_float2.

I just used a version of the paper's algorithm modified for doubles, and that was enough heckin trouble to debug, so while I'd love to learn what those assembly instructions are doing, I'm on a plane and don't plan to spend future time on this. <3

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <unistd.h>

#include "rand.h"

/* Get the total of user time and system time used by this process.
*/
double get_seconds() {
    double user, sys;
    struct rusage r;

    getrusage(RUSAGE_SELF, &r);

    user = r.ru_utime.tv_sec * 1000.0 + r.ru_utime.tv_usec / 1000.0;
    sys = r.ru_stime.tv_sec * 1000.0 + r.ru_stime.tv_usec / 1000.0;

    // printf("%lf\n", user);

    return user+sys;
}

/* Compute the total time used by a function.

iters: number of times to call the function
func: function to call
*/
double time_func(int iters, float(*func)())
{
    int i;
    float f;
    double t0, t1;

    srandom(time(NULL));

    t0 = get_seconds();
    for (i=0; i<iters; i++) {
        f = func();
    }
    t1 = get_seconds();
    return t1 - t0;
}

double time_func2(int iters, double(*func)())
{
    int i;
    double d;
    double t0, t1;

    srandom(time(NULL));

    t0 = get_seconds();
    for (i=0; i<iters; i++) {
        d = func();
    }
    t1 = get_seconds();
    return t1 - t0;
}


int main(int argc, char *argv[])
{
    double time;
    int iters = 100000000;
    int seed = 17;

    time = time_func(iters, dummy);
    printf("%f ms \t dummy\n", time);

    time = time_func(iters, dummy2);
    printf("%f ms \t dummy2\n", time);

    time = time_func(iters, random_float);
    printf("%f ms \t random_float\n", time);

    time = time_func(iters, my_random_float);
    printf("%f ms \t my_random_float\n", time);

    time = time_func(iters, my_random_float2);
    printf("%f ms \t my_random_float2\n", time);

    time = time_func(iters, random_float);
    printf("%f ms \t random_float\n", time);

    time = time_func(iters, random_double);
    printf("%f ms \t random_double\n", time);

    time = time_func2(iters, my_random_double);
    printf("%f ms \t my_random_double\n", time);

    return 0;
}
