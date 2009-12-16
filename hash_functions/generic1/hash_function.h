#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H

#include "rand_function.h"

typedef int hash_data[3];

static int shift;

void hash_init(hash_data a, int _shift) {
  shift = _shift;
  a[0] = ((int)hash_rand() << 1) + 1;
  a[1] = ((int)hash_rand() << 1) + 1;
  a[2] = ((int)hash_rand() << 1) + 1;
}

int hash(hash_data a, int key) {
  int h;
  h = (a[0]*key) ^ (a[1]*key) ^ (a[2]*key);
  h = (unsigned int)h >> shift;
  return h;
}

#endif
