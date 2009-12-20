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
  fprintf(stderr, "usage: %s [-m min_size] [-r rand_file] [-i] [-ci] [-c command_file]\n", av[0]);
  exit(1);
}

void read_index_interactive(char *index) {
  int r;
  char c;
  int n;
  char s[2];
  *index = 0;
  fputs("> ", stdout);
  r = scanf("%1s", s);
  *index = s[0];
  if(r != 1) exit(1);
}

void read_index(char *index, FILE *in) {
  int r;
  char c;
  int n;
  char s[2];
  *index = 0;
  if(fread(index,1,1,in) != 1) {
    if(ferror(in)) exit(1);
    else if(feof(in)) exit(0);
  }
}

void read_key_interactive(int *key) {
  int r;
  char c;
  *key = 0;
  fputs(": ", stdout);
  r = scanf("%d", key);
  if(r != 1) exit(1);
}

void read_key(int *key) {
  int r;
  char c;
  *key = 0;
  if(fread(key,sizeof(int),1,stdin) != sizeof(int)) {
    if(ferror(stdin)) exit(1);
    else if(feof(stdin)) exit(0);
  }
  /*printf("{key=%d}\n", key);*/
}

static dict_ptr D;
static int min_size;

void original_interface() {
  char index;
  int key;
  boolean found;
  int i;
  if (DETAIL) { printf("dch"); fflush(stdout); }

  while (fread(&index,1,1,stdin)==1) {
    switch(index) {
    case '1' : { /* Reset counters */
      break;
    }
    case '3' : { /* Insert element */
      fread(&key,sizeof(int),1,stdin);
      found = insert(D, key);
      if (DETAIL) {
        if(found) printf("1"); else printf("0");
      }
      break;
    } 
    case '4':  { /* Lookup element */ 
      fread(&key,sizeof(int),1,stdin);
      found = lookup(D, key);

      if (DETAIL) {
        if(found) printf("1"); else printf("0");
      }
      break; 
    }
    case '5':  { /* Delete element */
      fread(&key,sizeof(int),1,stdin);
      found = delete(D, key);
      if (DETAIL) {
        if(found) printf("1"); else printf("0");
      }
      break;
    }
    case '9':  { /* Retrieve size */
      i = size(D);
      if (DETAIL) printf ("%d", i);
      break;
    } 
    case '0':  { /* Clear dictionary */
      clear(D,min_size);
      if (DETAIL) printf("clr"); 
      break; 
    }
    }
  }
}

void interactive_interface() {
  int i;
  char index;
  int key;
  boolean found;
  while(1) {
    read_index_interactive(&index);
    switch(index) {
    case '1' : /* Reset counters */
      break;
    case '3': /* Insert element */
      read_key_interactive(&key);
      found = insert(D, key);
      if(found) printf("1\n");
      else printf("0\n");
      break;
    case '4': /* Lookup element */ 
      read_key_interactive(&key);
      found = lookup(D, key);
      if(found) printf("1\n");
      else printf("0\n");
      break; 
    case '5': /* Delete element */
      read_key_interactive(&key);
      found = delete(D, key);
      if(found) printf("1\n");
      else printf("0\n");
      break;
    case '9': /* Retrieve size */
      i = size(D);
      printf ("%d\n", i);
      break;
    case '0': /* Clear dictionary */
      clear(D,min_size);
      break; 
    }
  }
}

void command_interface(char *command_fn) {
  int i;
  char index;
  int key;
  boolean found;
  FILE *cf;
  cf = fopen(command_fn, "r");
  if(!cf) {
    fprintf(stderr, "failed to open command file.\n");
    exit(1);
  }
  while(1) {
    read_index(&index, cf);
    switch(index) {
    case '1' : /* Reset counters */
      break;
    case '3': /* Insert element */
      read_key(&key);
      found = insert(D, key);
      if(found) printf("1\n");
      else printf("0\n");
      break;
    case '4': /* Lookup element */ 
      read_key(&key);
      found = lookup(D, key);
      if(found) printf("1\n");
      else printf("0\n");
      break; 
    case '5': /* Delete element */
      read_key(&key);
      found = delete(D, key);
      if(found) printf("1\n");
      else printf("0\n");
      break;
    case '9': /* Retrieve size */
      i = size(D);
      printf ("%d\n", i);
      break;
    case '0': /* Clear dictionary */
      clear(D,min_size);
      break; 
    }
  }
}

void insertall_interface() {
  int key;
  boolean found;
  while(1) {
    read_key(&key);
    found = insert(D, key);
    if(found) printf("1");
    else printf("0");
  }
}

/* Driver reads commands, calls routines, reports */
int main(int argc, char **argv)
{
  int argi;
  int interactive = 0;
  int insertall = 0;
  char *rand_fn = 0;
  char *command_fn = 0;

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
    } else if(!strcmp("-i", argv[argi])) {
      argi++;
      interactive = 1;
    } else if(!strcmp("-ci", argv[argi])) {
      argi++;
      insertall = 1;
    } else if(!strcmp("-c", argv[argi])) {
      argi++;
      if(argi == argc) usage(argv);
      command_fn = argv[argi++];
    } else {
      usage(argv);
    }
  }

  hash_rand_init(rand_fn);

  /* Perform dictionary operations */
  D = construct_dict(min_size);

  if(interactive) interactive_interface();
  else if(insertall) insertall_interface();
  else if(command_fn) command_interface(command_fn);
  else original_interface();
  
  destruct_dict(D);

  return 0;
}
