/*
Simple adder for Software Systems SP2019
Homework 2

By Jane Sieving
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int running = 1;
int error = 0;

long long int getInput() {
  char str[18]; // actually 15 digits because of how this is handled
  char* read = fgets(str, sizeof(str), stdin);
  char* end;
  if (read == NULL) { // Ctrl-D pressed
    running = 0;
    return -1;
  }
  long long int num = strtoll(read, &end, 10);
  if ((int) strlen(read) == (int) sizeof(str)-1) { // input too long
    running = 0;
    error = 1;
    return -1;
  }
  if (num == 0) { // input not a number... or it's just 0, but better safe than sorry I guess
    running = 0;
    error = 2;
    return -1;
  }
  return num;
}

void addNums(long long int nums[]) {
  long long int total = 0;
  for (int i = 0; nums[i] > 0; i++) {
    total += nums[i];
  }
  printf("Total: %lli\n", total);
}

int main () {
  long long int nums[20];
  int i = 0;
  puts("________________________________________________________________________________\n");
  puts("Type numbers to add. Press return to add a new number, Ctrl-D to calculate sum.\n");
  while (running) {
    int num = getInput();
    nums[i] = num;
    i++;
    if (i >= (sizeof(nums)/sizeof(long long int))) {
      puts("Input array length met: you've done enough math for today.\n");
      running = 0;
      break;
    }
  }
  if (error == 0) {
    addNums(nums);
  } else if (error == 1) {
    puts("Input exceeded the maximum number of digits. Exiting....\n");
  } else {
    puts("Input was not a correctly formatted natural number. Exiting....\n");
  }
  return 0;
}
