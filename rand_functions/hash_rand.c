#include <stdio.h>
#include <stdlib.h>

static FILE *rand_f;

void hash_rand_init(char *fn) {
  rand_f = fopen(fn, "r");
  if(rand_f) {
    //buffer ?
  } else {
    fprintf(stderr, "error: failed to open random data file [%s]\n", fn);
    exit(1);
  }
}

int hash_rand() {
  int r;
  if(fread(&r, sizeof(int), 1, rand_f) != 1) {
    if(ferror(rand_f)) {

    } else if (feof(rand_f)) {

    }
  } else {
    return r;
  }
}
