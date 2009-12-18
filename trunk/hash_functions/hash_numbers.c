#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

//Add a specific hash function directory to the include path
#include <hash_function.c>
int hash_fun_data[RAND_SIZE_USED];
int hash_rand(){
  static int pos=0;
  //printf("hash_rand %d %d\n", pos, hash_fun_data[pos]);
  return hash_fun_data[pos++];
}

int main(int argc, char *argv[]) {
  if(argc < 3){
    fprintf(stderr, "Usage: %s infile.bin outfile.bin\n", argv[0]);
    fprintf(stderr, "Note: compile with different hash functions");
    return 1;
  }

  const char* inFileName = argv[1];
  const char* outFileName = argv[2];
  FILE * inFile;
  inFile = fopen ( inFileName , "rb" );
  if(!inFile){
    fprintf(stderr, "Could not open file: %s\n", inFileName);
    return 1;
  }
  int written;
  unsigned int value;
  int data_pos=0;
  int read;
  do{
    read = fread((void*)&value, 1, 4, inFile);
    if(read == 4){
      hash_fun_data[data_pos++] = value;
    }
  }while(read>0 && data_pos < RAND_SIZE_USED);
  if(data_pos != RAND_SIZE_USED){
    fprintf(stderr, "Not enough values to initialized the hash function. Run with a larger input data file\n");
    return 1;
  }
  //Close and re-open
  fclose(inFile);
  inFile = fopen ( inFileName , "rb" );

  FILE * outFile;
  outFile = fopen ( outFileName , "wb" );
  if(!outFile){
    fprintf(stderr, "Could not open file: %s\n", outFileName);
    return 1;
  }
  hash_data data;
  hash_init(data);
  
  do{
    read = fread((void*)&value, 1, 4, inFile);
    if(read == 4){
      value = hash(data, value, 0xFFFFFFFF);
      written = fwrite((const void*)&value, 1, 4, outFile);
      //printf("written: %d\n", written);
    }
  }while(read>0);
  
  fclose(inFile);
  fclose (outFile);

  printf("Succesfully hashed %s and wrote output values to %s\n", inFileName, outFileName);
  return 0;
}
