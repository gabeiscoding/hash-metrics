#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>

#include "driver.h"

/*----------------------------------------------*/
/* Dictionary driver based on the DIMACS driver */
/* by Catherine C. McGeoch                      */
/*                                              */
/* Modified by Rasmus Pagh and Flemming Rodler  */
/* June 2001.                                   */ 
/*----------------------------------------------*/

void usage(char **av) {
  fprintf(stderr, "usage: %s [-m min_size] rand_file.bin\n", av[0]);
  exit(1);
}

void read_index(char *index) {
  int r;
  char c;
  int n;
  char s[2];
  *index = 0;
  if(INTERACTIVE) {
    fputs("> ", stdout);
    r = scanf("%1s", s);
    *index = s[0];
    if(r != 1) exit(1);
  } else {
    if(fread(index,1,1,stdin) != 1) {
      if(ferror(stdin)) exit(1);
      else if(feof(stdin)) exit(0);
    }
  }
}

void read_key(int *key) {
  int r;
  char c;
  *key = 0;
  if(INTERACTIVE) {
    fputs(": ", stdout);
    r = scanf("%d", key);
    if(r != 1) exit(1);
  } else {
    if(fread(key,sizeof(int),1,stdin) != sizeof(int)) {
      if(ferror(stdin)) exit(1);
      else if(feof(stdin)) exit(0);
    }
  }
}

/* Driver reads commands, calls routines, reports */
int main(int argc, char **argv)
{
  dict_ptr  D;
  int key;
  boolean found;
  char index;
  int i, min_size;
  int argi;
  char *rand_fn = 0;

  min_size = 1024;

  argi = 1;
  while(argi != argc) {
    if(!strcmp("-m", argv[argi])) {
      argi++;
      if(argi == argc) usage(argv);
      min_size = atoi(argv[argi++]);
    } else if(!strcmp("-r", argv[argi])) {
      argi++;
      if(argi == argc) usage(argv);
      rand_fn = argv[argi++];
    } else {
      usage(argv);
    }
  }
  if(!rand_fn){
    usage(argv);
    return 1;
  }
  int size = hash_rand_init(rand_fn);
  if(size > min_size){
    fprintf(stderr, "min_size is less than than the # random values in the random value bin file\n");
    return 1;
  }
  printf("Read %d random values from %s\n", size, rand_fn);
  
  hash_data data;
  hash_init(data);
  hash_rand_reset_counter();
  
  /* Perform dictionary operations */
  D = construct_dict(min_size);
  if (DETAIL) { printf("dch"); fflush(stdout); }

  //Start benchmark
  clock_t s_clock = clock();
  time_t  s_time; time(&s_time);
  
  for(i=0; i<size; i++){
    key = hash_rand();
    found = insert(D, key);
    if (DETAIL) {
      if(found) printf("1");
      else printf("0");
    }
    if(!found)
      printf("FAILED INSERT ITEM %d KEY %d\n", i+1, key);
  }
  if(DETAIL) printf("\n");
  hash_rand_reset_counter();
  printf("Inserted %d keys\n", size);
  
  for(i=0; i<size; i++){
    key = hash_rand();
    found = lookup(D, key);
    if (DETAIL) {
      if(found) printf("1");
      else printf("0");
    }
    if(!found)
      printf("FAILED LOOKUP ITEM %d KEY %d\n", i+1, key);
  }
  if(DETAIL) printf("\n");
  hash_rand_reset_counter();
  printf("Looked up %d keys\n", size);

  for(i=0; i<size/2; i++){
    key = hash_rand();
    found = delete(D, key);
    if (DETAIL) {
      if(found) printf("1");
      else printf("0");
    }
    if(!found)
      printf("FAILED DELETE ITEM %d KEY %d\n", i+1, key);
  }
  if(DETAIL) printf("\n");
  hash_rand_reset_counter();
  printf("Deleted %d keys\n", size/2);

  for(i=0; i<size/2; i++){
    key = hash_rand();
    found = insert(D, key);
    if (DETAIL) {
      if(found) printf("1");
      else printf("0");
    }
  }
  if(DETAIL) printf("\n");
  hash_rand_reset_counter();
  printf("Inserted %d keys\n", size/2);

  for(i=0; i<size; i++){
    key = hash_rand();
    found = lookup(D, key);
    if (DETAIL) {
      if(found) printf("1");
      else printf("0");
    }
    if(!found)
      printf("FAILED LOOKUP ITEM %d KEY %d\n", i+1, key);
  }
  if(DETAIL) printf("\n");
  hash_rand_reset_counter();
  printf("Looked up %d keys\n", size);

  //End timer
  clock_t e_clock = clock();
  time_t  e_time; time(&e_time);

  //Print time metrics
  printf("clocks: %d\n", e_clock - s_clock);
  printf("secs: %.21f\n", difftime(e_time, s_time));
  
  destruct_dict(D);

  return 0;
}
