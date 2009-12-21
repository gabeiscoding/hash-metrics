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

HASH_SCHEMES="cuckoo
linear
two-choice"

RUNS="1
2
3"

bin_dir=bin-bench

N=33554432

for run in $RUNS
do
  for scheme in $HASH_SCHEMES
    do
    for hash in $HASH_FUNCTIONS
      do
      for r in $RAND_FUNCTIONS
	do
	echo -e -n "$scheme\t$hash\t$r\t" >> results/bench_results_${run}.txt
	echo "./$bin_dir/${scheme}_${hash} -bench -d ../enwik9.3_64mb -m $N -r ./data/rand_${r}.bin" >> results/bench_err_${run}.txt
	./$bin_dir/${scheme}_${hash} -bench -d ../enwik9.3_64mb -m $N -r ./data/rand_${r}.bin 1>> results/bench_results_${run}.txt 2>> results/bench_err_${run}.txt
	echo -e "" >> results/bench_results_${run}.txt
      done
    done
  done
done
