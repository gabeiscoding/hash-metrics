#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "rand.h"

int main(int argc, char *argv[]) {
  int written;
  int i;
  int n=1000;
  for(i = 1; i < argc; i++) {
    if(strstr(argv[i], "-g=") != NULL)
      //random number generator function:
      set_kind(atoi(argv[i] + 3));
    else if(strstr(argv[i], "-n=") != NULL)
      //size
      n = (atoi(argv[i] + 3));
    //else
    //  printf("Unknown command: %s\n", argv[i]);
  }
  const char* filename = argv[argc-1];
  //init_with_TOD();
  init(12345689);
  if(strcmp(filename,"stdout")==0){
    for(i=0; i<n; i++){
      unsigned int r = (unsigned int)(unif_rand()*INT_MAX);
      printf("%u\n", r);
    }
  }else{
    FILE * pFile;
    pFile = fopen ( filename , "wb" );
    if(!pFile){
      fprintf(stderr, "Could not open file: %s\n", filename);
      return 1;
    }
    
    for(i=0; i<n; i++){
      unsigned int r = (unsigned int)(unif_rand()*INT_MAX);
      written = fwrite((const void*)&r, 1, 4, pFile);
      //printf("written: %d\n", written);
    }
    fclose (pFile);
  }
  return 0;
}
