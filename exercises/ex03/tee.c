/*
tee.c : a simplified implementation of the 'tee' command line tool.

Takes optional arguments and a file name. Allows the user to type lines which
are both printed back to the terminal and written to the specified file. When
the EOF character is read (Ctrl+D), the input is saved and the program exits.

Created by Jane Sieving for Software Systems at Olin College.
2/21/19

*/

#include <stdio.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    char* file_name;
    char line[80];
    char ch;
    int a;
    int b;
    char* mode;
    mode = "w";

    /* Check for a file name in the last command line argument */
    if (argc > 1) {
        file_name = argv[argc-1];
        argc--;
    } else {
        puts("Output file name not specified!\n");
        return 1;
    }

    /* Handle the options, specifically -a to open the file in append mode.
     * I don't know how to implement the others yet, or really if we were
     * supposed to.
     */
    while ((ch = getopt(argc, argv, "aip")) != EOF) {
        switch(ch) {
          case 'a':
            mode = "a";
            break;
          default:
            printf("Unknown options: '%s'\n", optarg);
            return 1;
        }
        argc -= optind;
        argv += optind;
    }
    /* Open the file in the appropriate mode */
    FILE* out_file = fopen(file_name, mode);

    /* Take lines of input separated by return (newline).
     * Print the input and write it to the specified file.
     * Quit when Ctrl+D is pressed.
     */
    while (fgets(line, 80, stdin) != NULL) {
      printf("%s", line);
      fprintf(out_file, "%s", line);
    }
    printf("Saving to %s.\n", file_name);
    fclose(out_file);
    return 0;
}
