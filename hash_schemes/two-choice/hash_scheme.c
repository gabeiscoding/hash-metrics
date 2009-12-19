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
  int min_chainsize;
};

dict *alloc_dict(int tablesize) {
  dict *d;
  int i;
  int default_chain_size;
  
  d = calloc(1,sizeof(dict));
  d->size = 0;
  d->min_chainsize = 2;
  d->tablesize = tablesize;
  d->minsize =  (2*tablesize)/5;  
  d->kmask = tablesize - 1;

  if(!(d->t = calloc(tablesize, sizeof(chain)))) {
    fprintf(stderr,"Error while allocating mem for t\n");
    exit(0);
  }
  for(i=0; i<tablesize; i++) {
    d->t[i].maxsize = d->min_chainsize;
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
  *d = *dnew;
  free(dnew);
}

dict *construct_dict(int min_size) {
  return alloc_dict(min_size);
}

int insert(dict *d, int key) { 
  int hkey1;
  int hkey2;
  chain *chv[2], *ch;
  int i;

  hkey1 = hash(d->h1, key) & d->kmask;
  hkey2 = hash(d->h2, key) & d->kmask;

  chv[0] = &d->t[hkey1];
  chv[1] = &d->t[hkey2];

  ch = chv[1]->size < chv[0]->size ? chv[1] : chv[0];

  for(i=0; i<ch->size; i++)
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
  chain *chv[2];
  int ci;

  hkey1 = hash(d->h1, key) & d->kmask;
  hkey2 = hash(d->h2, key) & d->kmask;

  chv[0] = &d->t[hkey1];
  chv[1] = &d->t[hkey2];

  for(ci=0; ci<2; ci++)
    for(i=0; i<chv[ci]->size; i++)
      if(chv[ci]->c[i].key == key) return 1;

  return 0;
}

int delete(dict *d, int key) {
  int hkey1, hkey2;
  int i;
  int ci;
  chain *chv[2];

  hkey1 = hash(d->h1, key) & d->kmask;
  hkey2 = hash(d->h2, key) & d->kmask;

  chv[0] = &d->t[hkey1];
  chv[1] = &d->t[hkey2];

  for(ci=0; ci<2; ci++)
    for(i=0; i<chv[ci]->size; i++) {
      if(chv[ci]->c[i].key == key) {
        d->size--;
        chv[ci]->size--;
        if(!i) d->nr_chains--;
        if(d->nr_chains < d->minsize) rehash(d, d->tablesize/2);
        else if(d->min_chainsize > chv[ci]->size && chv[ci]->size < chv[ci]->maxsize/2) {
          chv[ci]->c = realloc(chv[ci]->c, chv[ci]->maxsize/2);
          chv[ci]->maxsize = chv[ci]->maxsize/2;
        }
        for(; i<chv[ci]->size; i++)
          chv[ci]->c[i] = chv[ci]->c[i+1];
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
