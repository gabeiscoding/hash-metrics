PRNGs. main.c and rand.c implemented 7 PRNG that generated unisigned integers from 0 to MAX_INT (2^32-1) and writes them to a binary file.

No makefile, but you can compile a write-rand:

gcc -c main.c -o main.o 
gcc -c rand.c -o rand.o
gcc -o write-rand main.o rand.o

You can run write write-rand
./write-rand -g=[0-7] -n=num fileout.bin

By default the number written is 1000, if the output file (last parameter) is "stdout" it prints the numbers to screen.

The -g specifies the algorithm. 4 does not work.

from rand.h
 0 = WICHMANN_HILL
 1 = MARSAGLIA_MULTICARRY
 2 = SUPER_DUPER
 3 = MERSENNE_TWISTER
 4 = KNUTH_TAOCP, //Not working, same as KNUTH_TAOCP2 but with different initialization
 5 = KNUTH_TAOCP2,
 6 = LIBC,
 7 = PRAND,


For reference, read_bin.c shows how to read the binary files written.

gcc -o read_bin read_bin.c

./read_bin fileout.bin

will print the integers stored in fileout.bin
