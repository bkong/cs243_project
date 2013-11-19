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
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>

#define MAX_BENCHMARKS 10
#define BENCHMARK_FOLDER "./src/benchmarks/"

static const char* EXT = ".bnch";

static void __run_benchmark(char *benchmark, int iterations);
static int __is_benchmark_file(const char *name);
static char **__get_benchmark_names();

extern int errno;


/* private function to get clock cycle count */
static uint64_t __get_clk()
{
	unsigned int lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

int main(int argc, char *argv[])
{
		if(argc == 1){
			printf("Usage: %s /path/to/executable\n", argv[0]);
			return;
		}
    uint64_t start = __get_clk();
        // Spawn child process
        pid_t pid = fork();
        if (pid == 0)   // Child process
        {
            if(execv(argv[1], argv)<0);{
                    printf("Errno is %d\n", errno);
            }
            exit(-1);
        }
        else // Parent Process
        {
            waitpid(pid, 0, 0); // Wait for child
        }
        uint64_t end = __get_clk();
        
        double total_clk = ((end - start)/1000000.0);
        printf("Total cycles is %f million\n", total_clk);
} // end main method



