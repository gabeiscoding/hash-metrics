/*
 * based on cuckoo.c
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

struct dict {
  int size;
  int tablesize;
  int minsize;
  int nr_chains;
  chain *t;
  int kmask;
  hash_data h;
  int min_chainsize;
};

dict *alloc_dict(int tablesize) {
  dict *d;
  int i;
  
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
    if(!(d->t[i].c = calloc(d->min_chainsize, sizeof(cell)))) {
      fprintf(stderr,"Error while allocating mem for chain\n");
      exit(0);
    }
  }
    
  hash_init(d->h);

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
  int hkey;
  chain *ch;
  int i;

  hkey = hash(d->h, key) & d->kmask;

  ch = &d->t[hkey];

  for(i=0; i<ch->size; i++)
    if(ch->c[i].key == key) return 0;

  if(ch->maxsize == i) {
    void *tmp = malloc(sizeof(cell) * ch->maxsize * 2);
    if(tmp) {
      memcpy(tmp, ch->c, ch->maxsize/2);
      free(ch->c);
      ch->c = tmp;
    } else {
      fprintf(stderr, "error: malloc failed at %s:%d\n", __FILE__, __LINE__);
      //kill(0, 2);
      exit(1);
    }
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
  int hkey;
  int i;
  chain *ch;

  hkey = hash(d->h, key) & d->kmask;

  ch = &d->t[hkey];

  for(i=0; i<ch->size; i++)
    if(ch->c[i].key == key) return 1;
  return 0;
}

int delete(dict *d, int key) {
  int hkey;
  int i;
  chain *ch;

  hkey = hash(d->h, key) & d->kmask;

  ch = &d->t[hkey];

  for(i=0; i<ch->size; i++) {
    if(ch->c[i].key == key) {
      d->size--;
      ch->size--;
      if(!i) d->nr_chains--;
      if(d->nr_chains < d->minsize) rehash(d, d->tablesize/2);
      else if(d->min_chainsize > ch->size && ch->size < ch->maxsize/2) {
        void *tmp = malloc(sizeof(cell) * ch->maxsize/2);
        if(tmp) {
          memcpy(tmp, ch->c, ch->maxsize/2);
          free(ch->c);
          ch->c = tmp;
        } else {
          fprintf(stderr, "error: malloc failed at %s:%d\n", __FILE__, __LINE__);
          //kill(0, 2);
          exit(1);
        }
        ch->maxsize = ch->maxsize/2;
      }
      for(; i<ch->size; i++)
        ch->c[i] = ch->c[i+1];
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
