/* test_benchmark: another benchmark */
#include <stdio.h>

int main(int argc, char **argv)
{
    int i;
    for(i=0;i<10;i++)
    {
        printf("Benchmarking module %d...\n", i);
    }
}

