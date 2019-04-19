/* Example from Head First C.

Downloaded from https://github.com/twcamper/head-first-c

Modified by Allen Downey.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <wait.h>


void error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int main(int argc, char *argv[])
{
  int status;
  pid_t pid;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <search phrase>\n", argv[0]);
        return 1;
    }
    const char *PYTHON = "/usr/bin/python2";
    const char *SCRIPT = "rssgossip.py";
    char *feeds[] = {
        "http://www.nytimes.com/services/xml/rss/nyt/Africa.xml",
        "http://www.nytimes.com/services/xml/rss/nyt/Americas.xml",
        "http://www.nytimes.com/services/xml/rss/nyt/MiddleEast.xml",
        "http://www.nytimes.com/services/xml/rss/nyt/Europe.xml",
        "http://www.nytimes.com/services/xml/rss/nyt/AsiaPacific.xml"
    };
    int num_feeds = 5;
    char *search_phrase = argv[1];
    char var[255];

    for (int i=0; i<num_feeds; i++) {
        pid = fork();
        // check for an error in fork
        if (pid == -1) {
            error("Fork failed.");
        }

        if (pid == 0) { // child code
            sprintf(var, "RSS_FEED=%s", feeds[i]);
            char *vars[] = {var, NULL};

            int res = execle(PYTHON, PYTHON, SCRIPT, search_phrase, NULL, vars);
            // check for an error in execle
            if (res == -1) {
                error("Can't run script.");
            }
            exit(0); // everything is fine
        }
    }
    // parent code
    for (int i=0; i<num_feeds; i++) {
        pid = wait(&status);
        // check for an error in wait
        if (pid == -1) {
            error("Wait failed.");
        }
        status = WEXITSTATUS(status);
        if (status) { // if a process did not exit normally
            printf("Process %d exited unsuccessfully with error code %d.\n", pid, status);
        }
    }
    return 0;
}
