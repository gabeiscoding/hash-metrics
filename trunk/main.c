/*
 * cs6150 Fall 2009 Project
 * Christopher Earl
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rand.h"

/*
 * Globals for this file:
 */
int randGen_function = 0;
int hash_function = 0;
int empTest_function = 0;
int runType = 0;

/*
 * hash function methods:
 */
void insert(int value);
void lookup(int value);

/*
 * random number generator function methods:
 */
void setSeed(int seed);
int next();

/*
 * empirical test methods:
 */
int run();


int main(int argc, char *argv[]) {
  int i;

  for(i = 1; i < argc; i++) {
    if(strstr(argv[i], "-h=") != NULL)
      //hash function:
      hash_function = atoi(argv[i] + 3);
    else if(strstr(argv[i], "-g=") != NULL)
      //random number generator function:
      set_kind(atoi(argv[i] + 3));
    else if(strstr(argv[i], "-e=") != NULL)
      //empirical test function:
      empTest_function = atoi(argv[i] + 3);
    else if(strstr(argv[i], "-r=") != NULL)
      //run type:
      runType = atoi(argv[i] + 3);
    else
      printf("Unknown command: %s\n", argv[i]);
  }
  //init_with_TOD();
  init(12345689);
  for(i=0; i<1000; i++)
    printf("%f\n", unif_rand());
  return 0;
}

void insert(int value) {
  switch(hash_function) {
  case 0:
    //insert call for hash function 0 with value as parameter:
    break;
  default:
    printf("Unknown hash function: %d\n", hash_function);
    break;
  }
}

void lookup(int value) {
  switch(hash_function) {
  case 0:
    //look-up call for hash function 0 with value as parameter:
    break;
  default:
    printf("Unknown hash function: %d\n", hash_function);
    break;
  }
}


void setSeed(int seed) {
  switch(randGen_function) {
  case 0:
    //set seed for random number generator function 0 with seed as parameter:
    break;
  default:
    printf("Unknown random number generator function: %d\n", randGen_function);
    break;
  }  
}

int next() {
  //default is 0
  int answer = 0;
  
  switch(randGen_function) {
  case 0:
    //returns next random number from random number generator function 0:
    break;
  default:
    printf("Unknown random number generator function: %d\n", randGen_function);
    break;
  }
  
  return answer;
}


int run() {
  switch(empTest_function) {
  case 0:
    //runs empirical test function 0 with current random number generator:
    break;
  default:
    printf("Unknown empirical test: %d\n", empTest_function);
    break;
  }
}
