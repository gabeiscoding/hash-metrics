typedef int hash_data[4][256];

void hash_init(hash_data a) {
  int i;
  for(i=0; i<32; i++) {
    a[i] = hash_rand();
  }
}

int hash(hash_data a, int key, int mask) {
  int h, i;
  for(i=0; i<4; ++i)
    h ^= a[i][((char*)&key)[i]];
  return h & mask;
}
