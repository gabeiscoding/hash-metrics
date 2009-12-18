#ifndef DDRIVER_H
#define DDRIVER_H

struct dict;
typedef struct dict dict;
typedef dict *dict_ptr;

#define TRUE 1
#define FALSE 0

typedef int boolean;

extern dict_ptr    construct_dict(int min_size); 
extern boolean     insert       (dict_ptr D, int key);
extern boolean     lookup       (dict_ptr D, int key); 
extern boolean     delete       (dict_ptr D, int key); 
extern int         keyval       (dict_ptr D, int key);
extern int         size         (dict_ptr D); 
extern void        clear        (dict_ptr D, int min_size); 
extern dict_ptr    destruct_dict(dict_ptr D); 

#include "rand_functions/hash_rand.h"
#include "hash_function.c"
#include "hash_scheme.c"

#endif 

