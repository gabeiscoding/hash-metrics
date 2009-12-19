#!/bin/bash

cc=gcc
cflags="$cflags -g -DDETAIL=1"

hash_scheme_dir=hash_schemes
hash_fn_dir=hash_functions
bin_dir=bin

libs=-lm

scheme=$1
hash=$2

[ "$3" == i ] && cflags="$cflags -DINTERACTIVE=1"

includes="-I$hash_fn_dir/$hash -I$hash_scheme_dir/$scheme"

[ -d $bin_dir ] || mkdir $bin_dir

$cc $cflags -o $bin_dir/$1_$2$3 driver.c $libs $includes rand_functions/hash_rand.c
