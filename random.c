#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000000000000

int rand_txt(int argc, char *argv[]) {
  srand(time(NULL));
  int n = atoi(argv[1]);
  FILE *f = fopen(argv[2], "w");
  for (int i = 0; i < n; ++i) {
    int r = 1 + rand() / (RAND_MAX / MAX + 1);
    fprintf(f, "%d\n", r);
  }
  fclose(f);

  return 0;
}
