typedef int hash_data[32];

void hash_init(hash_data a) {
  int i;
  for(i=0; i<32; i++) {
    a[i] = hash_rand();
  }
}

int hash(hash_data a, int key, int mask) {
  int h, i;
  for (i=0; i<(4<<3); i+=8)
  {
    char k = ((char*)key)[i>>3];
    if(k&0x01) h ^= a[i+0];
    if(k&0x02) h ^= a[i+1];
    if(k&0x04) h ^= a[i+2];
    if(k&0x08) h ^= a[i+3];
    if(k&0x10) h ^= a[i+4];
    if(k&0x20) h ^= a[i+5];
    if(k&0x40) h ^= a[i+6];
    if(k&0x80) h ^= a[i+7];
  }
  return h & mask;
}
