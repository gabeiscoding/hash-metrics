#ifndef RAND_H_
#define RAND_H_

typedef enum {
    WICHMANN_HILL = 0,
    MARSAGLIA_MULTICARRY,
    SUPER_DUPER,
    MERSENNE_TWISTER,
    KNUTH_TAOCP, //Not working, same as KNUTH_TAOCP2 but with different initialization
    KNUTH_TAOCP2,
    LIBC,
    PRAND,
} RNGtype;

/* Different kinds of "N(0,1)" generators :*/
typedef enum {
    BUGGY_KINDERMAN_RAMAGE,
    AHRENS_DIETER,
    BOX_MULLER,
    USER_NORM,
    INVERSION,
    KINDERMAN_RAMAGE
} N01type;

typedef unsigned int Int32;
void set_kind(RNGtype kind);

/* Init current RNG state with the current seed */
void init(Int32 seed);
/* Init current RNG state with a Time of Day seed */
void init_with_TOD();

double unif_rand(void);

#endif
