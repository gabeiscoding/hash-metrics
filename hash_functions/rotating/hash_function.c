typedef int hash_data;

void hash_init(hash_data a) {
}

int hash(hash_data a, int key) {
  int h=0, i;
  for(i=0; i<4; ++i)
    h = (h<<4)^(h>>28)^((char*)&key)[i];
  return h;
}
