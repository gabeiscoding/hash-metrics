#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static FILE *rand_f;
static int *rand_buf;
static int ri;
static int rand_size;

void hash_rand_init(char *fn) {
  struct stat st;
  rand_f = fopen(fn, "r");
  if(rand_f) {
    fstat(fileno(rand_f), &st);
    rand_size = st.st_size;
    rand_buf = malloc(rand_size);
    for(ri=0; ri<rand_size/4; ri++) {
      if(fread(rand_buf+ri, sizeof(int), 1, rand_f) != 1) {
        if(ferror(rand_f)) {
          fprintf(stderr, "error: failed reading file [%s]\n", fn);
          exit(1);
        }
/*
        else if (feof(rand_f)) {
          
        }
*/
      }
    }
  } else {
    srand(0);
    rand_size = 4096*4;
    rand_buf = malloc(rand_size);
    for(ri=0; ri<rand_size/4; ri++) rand_buf[ri] = rand();
  }
  ri=0;
}

int hash_rand() {
  return rand_buf[ri++];
}
