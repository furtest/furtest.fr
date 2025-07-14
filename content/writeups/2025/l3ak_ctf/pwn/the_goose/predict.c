#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
	srand(time(NULL) + 1);
	printf("%d", (rand() % 0x5b + 10));
	return 0;
}
