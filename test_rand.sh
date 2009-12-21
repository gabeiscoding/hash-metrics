#!/bin/bash
RAND_FUNCTIONS="0
1
2
3
5
6
7
8"

HASH_FUNCTIONS="generic
rotating
universal
zobrist"

N=1000000

for r in $RAND_FUNCTIONS
do
    ./diedir/diehard ./data/rand_${r}.bin t 16 > results/rand_diehard/rand_${r}.txt
    for h in $HASH_FUNCTIONS
    do
      ./diedir/diehard ./data/rand_${r}_hashed_${h}.bin t 16 > results/rand_diehard/rand_${r}_hashed_${h}.txt
    done
done
