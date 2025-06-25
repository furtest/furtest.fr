
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


char * encode(char*);

int main(void){
    int count = 0;
	char passwd[17] = {0};
	char *res = NULL;
	int test;
    long int part1 = 0x46f0b8f4eff0544d;
    long int part2 = 0x5fb844f850e415f3; 
	for(int c = 0; c < 16; c++){
		for(int i = 0; i < 256; ++i){
			passwd[c] = (char) i;
			res = encode(passwd);
			if(c < 9){
				test = memcmp(res, &part1, c+1);
			}else{
				test = memcmp(res+8, &part2, c-7);
			}
			if(test == 0){
                count++;
				printf("%c", (char) i);
				break;
			}
		}
	}
    if(count == 16){
        return 0;
    }
	return 1;
}
