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

N=262144
N1=512
N2=512

for r in $RAND_FUNCTIONS
do
    echo "Generating $N random numbers using random function $r"
    ./rand_functions/write-rand -g=${r} -n=${N} ./data/rand_${r}.bin
    echo "Building image from rand function $r bin"
    ./rand_to_bitmap $N1 $N2 ./data/rand_${r}.bin ./data/rand_${r}.xbm
    convert ./data/rand_${r}.xbm ./data/rand_${r}.png
    for h in $HASH_FUNCTIONS
    do
        echo "Hashing rand $r with hashing function $h"
        ./hash_functions/${h}_hash_numbers ./data/rand_${r}.bin ./data/rand_${r}_hashed_${h}.bin
        echo "Building image rand function $r hashed with $h bin"
        ./rand_to_bitmap $N1 $N2 ./data/rand_${r}_hashed_${h}.bin ./data/rand_${r}_hashed_${h}.xbm
        convert ./data/rand_${r}_hashed_${h}.xbm ./data/rand_${r}_hashed_${h}.png
    done
done
