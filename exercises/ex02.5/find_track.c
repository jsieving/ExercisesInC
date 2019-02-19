/* Example code for Exercises in C.

Modified version of an example from Chapter 2.5 of Head First C.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define NUM_TRACKS 5

char tracks[][80] = {
    "So What",
    "Freddie Freeloader",
    "Blue in Green",
    "All Blues",
    "Flamenco Sketches"
};


// Finds all tracks that contain the given string.
//
// Prints track number and title.
void find_track(char search_for[])
{
    int i;
    for (i=0; i<NUM_TRACKS; i++) {
        if (strstr(tracks[i], search_for)) {
            printf("Track %i: '%s'\n", i, tracks[i]);
        }
    }
}

// Finds all tracks that match the given pattern.
//
// Prints track number and title.
void find_track_regex(char pattern[])
{
    int i;
    int errno;
    char errbuf[80];
    regex_t preg;
    int res;
    int match = 0;
    errno = regcomp(&preg, pattern, REG_EXTENDED | REG_NOSUB);
    /* Handle errors from regcomp, or search tracks for the compiled regexp. */
    if (errno) {
        regerror(errno, &preg, errbuf, sizeof(errbuf));
        printf("%s\n", errbuf);
        exit(1);
    } else {
        for (i=0; i<NUM_TRACKS; i++) {
            res = regexec(&preg, tracks[i], 0, NULL, 0);
            if (res == 0) {
                printf("Track %i: '%s'\n", i, tracks[i]);
                match = 1;
            /* Handle errors except NOMATCH - we don't always expect a match. */
            } else if (res != REG_NOMATCH) {
              regerror(res, &preg, errbuf, sizeof(errbuf));
              printf("%s\n", errbuf);
              exit(1);
            }
        }
        if (match == 0) {
            puts("No matches found.");
        }
    }
    regfree(&preg);
}

// Truncates the string at the first newline, if there is one.
void rstrip(char s[])
{
    char *ptr = strchr(s, '\n');
    if (ptr) {
        *ptr = '\0';
    }
}

// Prompts the user to search for a string or regular expression,
// then prints any tracks containing the string or matching the
// regular expression.
int main (int argc, char *argv[])
{
    char search_for[80];

    /* take input from the user and search */
    printf("Search for: ");
    fgets(search_for, 80, stdin);
    rstrip(search_for);

    // find_track(search_for);
    find_track_regex(search_for);

    return 0;
}
