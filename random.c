#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "helpers.h"

int rand_txt(int argc, char *argv[]) {
  srand(time(NULL));
  int n = atoi(argv[1]);
  FILE *f = fopen(argv[2], "w");
  for (int i = 0; i < n; ++i) {
    long long int r = 1 + rand() / (RAND_MAX / MAX + 1);
    fprintf(f, "%d\n", r);
  }
  fclose(f);

  return 0;
}
