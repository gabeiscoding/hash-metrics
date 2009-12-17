#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int main(int argc, char *argv[]) {
  const char* filename = argv[argc-1];
  FILE * pFile;
  pFile = fopen ( filename , "rb" );
  if(!pFile){
    fprintf(stderr, "Could not open file: %s\n", filename);
    return 1;
  }
  unsigned int value;
  int read;
  do{
    read = fread((void*)&value, 1, 4, pFile);
    if(read == 4)
      printf("%u\n", value);
  }while(read>0);
  fclose(pFile);
}
