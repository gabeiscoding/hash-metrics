#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "driver.h"

/*----------------------------------------------*/
/* Dictionary driver based on the DIMACS driver */
/* by Catherine C. McGeoch                      */
/*                                              */
/* Modified by Rasmus Pagh and Flemming Rodler  */
/* June 2001.                                   */ 
/*----------------------------------------------*/

void usage(char **av) {
  fprintf(stderr, "usage: %s [-m min_size] {-r rand_file}\n", av[0]);
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
      if(ferror(stdin)) {}
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
    if(fread(&key,sizeof(int),1,stdin) != sizeof(int)) {
      if(ferror(stdin)) {}
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
      if(argi == argc) usage(argv);
      argi++;
      rand_fn = argv[argi++];
    } else {
      usage(argv);
    }
  }

  if(rand_fn) {
    hash_rand_init(rand_fn);
  } else {
    usage(argv);
  }

  /* Perform dictionary operations */
  D = construct_dict(min_size);
  if (DETAIL) { printf("dch"); fflush(stdout); }

  if(INTERACTIVE) puts("");

  while(1) {
    read_index(&index);
    switch(index) {
    case '1' : { /* Reset counters */
      break;
    }
    case '3' : { /* Insert element */
      read_key(&key);
      found = insert(D, key);
      if (DETAIL) {
        if(found) printf("1");
        else printf("0");
      }
      if(INTERACTIVE) puts("");
      break;
    } 
    case '4':  { /* Lookup element */ 
      read_key(&key);
      found = lookup(D, key);
      if (DETAIL) {
        if(found) printf("1");
        else printf("0");
      }
      if(INTERACTIVE) puts("");
      break; 
    }
    case '5':  { /* Delete element */
      read_key(&key);
      found = delete(D, key);
      if (DETAIL) {
        if(found) printf("1");
        else printf("0");
      }
      if(INTERACTIVE) puts("");
      break;
    }
    case '9':  { /* Retrieve size */
      i = size(D);
      if (DETAIL) printf ("%d", i);
      if(INTERACTIVE) puts("");
      break;
    } 
    case '0':  { /* Clear dictionary */
      clear(D,min_size);
      if (DETAIL) printf("clr"); 
      if(INTERACTIVE) puts("");
      break; 
    }
    }
  }
  
  destruct_dict(D);

  return 0;
}
