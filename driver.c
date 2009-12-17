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

static char *usage = "usage: %s [-m min_size] {-r rand_file}\n";

/* Driver reads commands, calls routines, reports */
int main(int argc, char **argv)
{
  dict_ptr  D;
  int key;
  boolean found;
  char index;
  int i, min_size;
  int argi;
  char *rand_fn;

  min_size = 1024;
  if(argc > 5) {
    fprintf(stderr, usage, argv[0]);
    exit(1);
  }

  argi = 1;
  while(argi != argc) {
    if(!strcmp("-m", argv[argi])) {
      min_size = atoi(argv[argi]);
      argi++;
    } else if(!strcmp("-r", argv[argi])) {
      rand_fn = argv[argi];
      argi++;
    }
  }

  if(rand_fn) {
    hash_rand_init(rand_fn);
  } else {
    fprintf(stderr, usage, argv[0]);
    exit(1);
  }

  /* Perform dictionary operations */
  D = construct_dict(min_size);
  if (DETAIL > 1) { printf("dch"); fflush(stdout); }

  while (fread(&index,1,1,stdin)==1) {
    switch(index) {
    case '1' : { /* Reset counters */
      break;
    }
    case '3' : { /* Insert element */
      fread(&key,sizeof(int),1,stdin);
      found = insert(D, key);
      if (DETAIL > 1) {
	if(found) printf("1"); else printf("0");
      }
      break;
    } 
    case '4':  { /* Lookup element */ 
      fread(&key,sizeof(int),1,stdin);
      found = lookup(D, key);

      if (DETAIL >= 1) {
	if(found) printf("1"); else printf("0");
      }
      break; 
    }
    case '5':  { /* Delete element */
      fread(&key,sizeof(int),1,stdin);
      found = delete(D, key);
      if (DETAIL >= 1) {
	if(found) printf("1"); else printf("0");
      }
      break;
    }
    case '9':  { /* Retrieve size */
      i = size(D);
      if (DETAIL >= 1) printf ("%d", i);
      break;
    } 
    case '0':  { /* Clear dictionary */
      clear(D,min_size);
      if (DETAIL > 1) printf("clr"); 
      break; 
    }
    }
  }
  
  destruct_dict(D);

  return 0;
}
