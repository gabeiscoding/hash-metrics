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
    echo "Generating $N random numbers using random function $r"
    ./bin/write_rand -g=${r} -n=${N} ./data/rand_${r}.bin
    echo "Building image from rand function $r bin"
    for h in $HASH_FUNCTIONS
    do
        echo "Hashing rand $r with hashing function $h"
        ./bin/${h}_hash_numbers ./data/rand_${r}.bin ./data/rand_${r}_hashed_${h}.bin
    done
done
