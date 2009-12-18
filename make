#!/bin/bash

cc=gcc
cflags="$cflags -g -DDETAIL=1"

hash_scheme_dir=hash_schemes
hash_fn_dir=hash_functions

#hash_scheme_linear=-Ihash_schemes/linear_probe
#hash_scheme_cuckoo=-ihash_schemes/cuckoo
#hash_fn_generic=-Ihash_functions/generic1
#hash_fn_universal=-Ihash_functions/universal
#hash_fn_zobrist=-Ihash_functions/zobrist

libs=-lm

scheme=$1
hash=$2

[ "$3" == i ] && cflags="$cflags -DINTERATIVE=1"

includes="-I$hash_fn_dir/$hash -I$hash_scheme_dir/$scheme"

$cc $cflags -o $1_$2$3 driver.c $libs $includes rand_functions/hash_rand.c
