#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int main(int argc, char *argv[]) {
  if(argc != 5){
    fprintf(stderr, "Usage: %s Width Heigh InFile.bin OutImage.xbm\n", argv[0]);
    return 1;
  }
  int w = atoi(argv[1]);
  int h = atoi(argv[2]);
  const char* filename = argv[3];
  FILE * pFile;
  pFile = fopen ( filename , "rb" );
  if(!pFile){
    fprintf(stderr, "Could not open file: %s\n", filename);
    return 1;
  }
  const char* outname = argv[4];
  FILE * oFile;
  oFile = fopen ( outname , "w" );
  if(!oFile){
    fprintf(stderr, "Could not open file: %s\n", outname);
    return 1;
  }
  fprintf(oFile, "#define xbm_width %d\n", w);
  fprintf(oFile, "#define xbm_height %d\n", h);
  fprintf(oFile, "static char xbm_bits[] = {\n");
  int num_bytes = (w*h)/8;
  int cur_byte = 0;
  unsigned int v1,v2,v3,v4,v5,v6,v7,v8;
  int r1,r2,r3,r4,r5,r6,r7,r8;
  do{
    //Construct a byte
    r1 = fread((void*)&v1, 1, 4, pFile);
    r2 = fread((void*)&v2, 1, 4, pFile);
    r3 = fread((void*)&v3, 1, 4, pFile);
    r4 = fread((void*)&v4, 1, 4, pFile);
    r5 = fread((void*)&v5, 1, 4, pFile);
    r6 = fread((void*)&v6, 1, 4, pFile);
    r7 = fread((void*)&v7, 1, 4, pFile);
    r8 = fread((void*)&v8, 1, 4, pFile);
    if(r1 != 4 || r2 != 4 || r3 != 4 || r4 != 4 ||r5 != 4 ||r6 != 4 || r7 != 4)
    {
      fprintf(stderr, "Not enough random numbers. In the middle of byte %d of %d\n", cur_byte, num_bytes);
      return 1;
    }
    if(r8 == 4){
      unsigned char b = 0;
      b ^= (v1%2) << 0;
      b ^= (v2%2) << 1;
      b ^= (v3%2) << 2;
      b ^= (v4%2) << 3;
      b ^= (v5%2) << 4;
      b ^= (v6%2) << 5;
      b ^= (v7%2) << 6;
      b ^= (v8%2) << 7;
      //printf("%d%d%d%d%d%d%d%d 0x%x\n", (v1%2),(v2%2),(v3%2),(v4%2),(v5%2),(v6%2),(v7%2),(v8%2),b);
      fprintf(oFile, "0x%x, ", b);
    }
    cur_byte++;
  }while(r8>0 && cur_byte < num_bytes);
  if(cur_byte != num_bytes){
    fprintf(stderr, "Not enough random numbers. Processed %d bytes instead of %d\n", cur_byte, num_bytes);
      return 1;
  }
  fclose(pFile);
  fprintf(oFile, "\n};\n");
  fclose(oFile);
}
