#include <stdio.h>

int verify(int res, int a, int b) {
  printf("%d x %d = %d\n", a, b, res);
  if (res == a*b) {
    return 1;
  }
  else {
    return 0;
  }
}

int mult(int a, int b) {
  int total = 0;
  for (int i=0; i<a; i++) {
    for (int j=0; j<b; j++) {
      total++;
    }
  }
  if (verify(total, a, b)) {
    return total;
  }
}

int main() {
    // int x = 5;
    printf("Hello, World!\n");
    printf("%d\n", mult(2,3));
    // printf("%d\n", x);
    // int y = x + 1;
    // printf("%d\n", y);
    return 0;
}
