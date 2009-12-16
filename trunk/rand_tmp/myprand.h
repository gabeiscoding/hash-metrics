/* Simple fast halfaway decent parallel random number generator
 * Period length 2^31 - 1, least significant bits not very random
 */

unsigned plural long myRandState;

/* seed and init generator */
#define sMyPRand(s) (myRandState = (s) ^ ((plural int)iproc << 15))

/* get a random number between 0 and 2^32 - 1 */
#define myPRand() (\
   myRandState = (1664525L*myRandState + 1013904223L))


