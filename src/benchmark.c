/***********************************************
 * Benchmark.c                                 *
 * Benchmarking suite for CS 241               *
 * Group members:                              *
 *   Tyler Howe, Jacob Heller, & Bailey Kong   *
 *                                             *
 * Use: Each benchmark file uses the           *
 *   reg_benchmark() to register that file's   *
 *   benchmark for use in the suite.  The      *
 *   user chooses a suite to run either on the *
 *   command line or at runtime via the        *
 *   terminal.                                 *
 **********************************************/
 
#include <stdio.h>
#include <stdint.h>
#include "include/benchmark.h"

static void __run_benchmark(Benchmark b);

int main(int argc, char** argv)
{
    int i;
    int bench_num = 0;
    
    if (argc <= 1)
    {
        // User needs to select which benchmark to run
        printf("Choose which benchmark to run:\n");
        for (i=1; i<=NUM_BENCHMARKS; i++)
        {
            printf("%d. %s\n", i, BENCHMARKS[i-1].name);
        }
        scanf("%d", &bench_num);
    }
    else
    {
        // User entered which benchmark as command line arg
        // TODO: allow benchmark name or number as arg
        if (sscanf(argv[1], "%d", &bench_num) != 1)
        {
            printf("Invalid benchmark number %s\n", argv[1]);
            return -1;
        }
    }

    if (bench_num > NUM_BENCHMARKS)
    {
        printf("Benchmark number %d does not exist\n", bench_num);
        return -1;
    }
    
    __run_benchmark(BENCHMARKS[bench_num-1]);
    return 0;
}

/* private function to get clock cycle count */
static uint64_t __get_clk()
{
	unsigned int lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

/* private function to run a benchmark */
static void __run_benchmark(Benchmark b)
{
    uint64_t start, end;
    
    start = __get_clk();
    b.run();
    end = __get_clk();
    printf("Total number of clock cycles: %d\n" , end - start);
}


