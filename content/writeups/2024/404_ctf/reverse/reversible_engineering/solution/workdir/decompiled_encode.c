#include <stdlib.h>
#include <stdint.h>
char * encode(long param_1)

{
  char bVar1;
  char bVar2;
  char *pvVar3;
  int local_c;
  
  pvVar3 = malloc(0x10);
  for (local_c = 0; local_c < 0x10; local_c = local_c + 1) {
    bVar1 = *(char *)(param_1 + local_c);
    bVar2 = bVar1 ^ (char)(((int)(uint)bVar1 >> 6 & (int)(uint)bVar1 >> 5 & 1U) << 3) ^
            (char)((bVar1 >> 6 & 1) << 2);
    bVar2 = bVar2 ^ (char)((bVar2 >> 2 & 1) << 5) ^ 4;
    bVar2 = bVar2 ^ (char)(((int)(uint)bVar2 >> 5 & (int)(uint)bVar2 >> 4 & 1U) << 3);
    bVar2 = bVar2 ^ (char)(((int)(uint)bVar2 >> 3 & bVar2 & 1) << 2) ^ 1 ^
            (char)((bVar1 >> 6 & 1) << 4) ^ 0x10;
    bVar2 = bVar2 ^ (bVar1 >> 7 & (char)((int)(uint)bVar2 >> 6) & 1) * '\x02';
    bVar2 = bVar2 ^ (char)(((int)(uint)bVar2 >> 3 & (int)(uint)bVar2 >> 1 & 1U) << 5) ^ 8;
    bVar2 = bVar2 ^ (char)((bVar2 >> 3 & 1) << 5);
    *(char *)((long)local_c + (long)pvVar3) =
         bVar2 ^ (char)(((uint)(bVar1 >> 7) & (int)(uint)bVar2 >> 4 & 1U) << 2) ^ 0x10 ^
         (bVar2 >> 5) << 7 ^ 2;
  }
  return pvVar3;
}

