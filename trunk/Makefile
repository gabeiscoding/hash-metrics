CC=gcc

HASH_SCHEME=-Ihash_schemes/cuckoo
HASH_FN=-Ihash_functions/generic1

INCLUDES=$(HASH_FN) $(HASH_SCHEME)
LIBS=-lm

cuckoo: rand_functions/hash_rand.o
	$(CC) -o $@ driver.c $(LIBS) $(INCLUDES) $<
