/*
 * based on cuckoo.c
 *
*/

#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<time.h>
#include<math.h>

typedef struct cell {       /* hash table cell type */ 
  int key; 
} cell;

typedef struct chain {
  int maxsize;
  int size;
  cell *c;
} chain;

struct dict {            /* dictionary type */ 
  int size;                 /* current size */
  int tablesize;            /* size of hash tables */
  int minsize,meansize;     /* rehash trigger sizes */
  int nr_chains;
  chain *t;
  int kmask;
  hash_data h1;
  hash_data h2;
};

dict *alloc_dict(int tablesize) {
  dict *d;
  int i;
  int default_chain_size;
  
  d = calloc(1,sizeof(dict));
  d->size = 0;
  d->tablesize = tablesize;

  d->meansize = 5*(2*tablesize)/12;
  d->minsize =  (2*tablesize)/5;  

  if(!(d->t = calloc(tablesize, sizeof(chain)))) {
    fprintf(stderr,"Error while allocating mem for t\n");
    exit(0);
  }
  for(i=0; i<tablesize; i++) {
    d->t[i].maxsize = default_chain_size;
    d->t[i].size = 0;
    if(!(d->t[i].c = calloc(default_chain_size, sizeof(cell)))) {
      fprintf(stderr,"Error while allocating mem for chain\n");
      exit(0);
    }
  }
    
  hash_init(d->h1);
  hash_init(d->h2);

  return d;
}

void rehash(dict* d, int new_size) {
  dict *dnew;
  int k;
  int i;

  dnew = alloc_dict(new_size);

  for(k = 0; k < d->tablesize; k++)
    for(i=0; i<d->t[k].size; i++)
      insert(dnew, d->t[k].c[i].key);

  free(d->t);
  dnew->size = d->size;
  *d = *dnew;
  free(dnew);
}

dict *construct_dict(int min_size) {
  return alloc_dict(min_size);
}

int insert(dict *d, int key) { 
  int hkey1;
  int hkey2;
  chain *ch1, *ch2, *ch;
  int i;

  hkey1 = hash(d->h1, key, d->kmask);
  hkey2 = hash(d->h2, key, d->kmask);

  ch1 = &d->t[hkey1];
  ch2 = &d->t[hkey2];

  ch = ch2->size < ch1->size ? ch2 : ch1;

  for(i=0; i<ch->size && ch->c[i].key; i++)
    if(ch->c[i].key == key) return 0;

  if(ch->size == i) {
    ch->c = realloc(ch->c, ch->maxsize*2);
    ch->maxsize *= 2;
  } else if (!i) {
    d->nr_chains++;
  }
  ch->c[i].key = key;
  ch->size++;
  d->size++;

  if(d->tablesize < d->nr_chains) rehash(d, 2*d->tablesize);
  return 1;
}

int lookup(dict* d, int key) {
  int hkey1, hkey2;
  int i;
  chain *ch1, *ch2;

  hkey1 = hash(d->h1, key, d->kmask);
  hkey2 = hash(d->h2, key, d->kmask);

  ch1 = &d->t[hkey1];
  ch2 = &d->t[hkey2];

  for(i=0; i<ch1->size && ch1->c[i].key; i++)
    if(ch1->c[i].key == key) return 1;
  for(i=0; i<ch2->size && ch2->c[i].key; i++)
    if(ch2->c[i].key == key) return 1;
  return 0;
}

int delete(dict *d, int key) {
  int hkey1, hkey2;
  int i;
  chain *ch1, *ch2;

  hkey1 = hash(d->h1, key, d->kmask);
  hkey2 = hash(d->h2, key, d->kmask);

  ch1 = &d->t[hkey1];
  ch2 = &d->t[hkey2];

  for(i=0; i<ch1->size && ch1->c[i].key; i++) {
    if(ch1->c[i].key == key) {
      d->size--;
      if(!i) d->nr_chains--;
//      if(d->size < d->minsize) ; /*rehash(d, d->tablesize/2);*/
      if(d->nr_chains < d->minsize) rehash(d, d->tablesize/2);
      for(; i<ch1->size-1 && ch1->c[i].key; i++)
        ch1->c[i] = ch1->c[i+1];
      ch1->c[i].key = 0;
      return 1;
    }
  }
  for(i=0; i<ch2->size && ch2->c[i].key; i++) {
    if(ch2->c[i].key == key) {
      d->size--;
      if(!i) d->nr_chains--;
//      if(d->size < d->minsize) ; /*rehash(d, d->tablesize/2);*/
      if(d->nr_chains < d->minsize) rehash(d, d->tablesize/2);
      for(; i<ch2->size-1 && ch2->c[i].key; i++)
        ch2->c[i] = ch2->c[i+1];
      ch2->c[i].key = 0;
      return 1;
    }
  }
  return 0;
}

int size(dict *d) {
  return d->size;
}

void clear(dict *d, int min_size) {
  dict *dnew;
  dnew = construct_dict(min_size);
  free(d->t);
  *d = *dnew;
  free(dnew);
}

dict *destruct_dict(dict *d) {
  free(d->t);
  free(d);
  return NULL;
}
