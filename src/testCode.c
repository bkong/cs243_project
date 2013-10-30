/* Hello World program */

#include <stdio.h>
#include <stdint.h>


uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

main()
{
	uint64_t start = rdtsc();
	int i;
	for(i=0; i<100; i++){
		printf("%d\n",i);
	}
	uint64_t end = rdtsc();
	printf("Start %llu, End %llu\n", start, end);
	printf("Total Time: %llu\n", end-start);
}
