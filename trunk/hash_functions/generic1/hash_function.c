#define RAND_SIZE_USED 4
typedef int hash_data[RAND_SIZE_USED];

void hash_init(hash_data a) {
  a[0] = (hash_rand() << 1) + 1;
  a[1] = (hash_rand() << 1) + 1;
  a[2] = (hash_rand() << 1) + 1;
  a[3] = (hash_rand() << 1) + 1;
}

int hash(hash_data a, int key, int mask) {
  int h;
  h = (a[0]*key) ^ (a[1]*key) ^ (a[2]*key) ^ (a[3]*key);
  return h & mask;
}
