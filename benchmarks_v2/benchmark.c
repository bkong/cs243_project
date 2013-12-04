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
#define BENCHMARK_FOLDER "./compiled/"
#define BENCHMARK_RESULT_FILE "./benchmark_results.csv"
#define ITERATIONS 5 // Number of times to run each benchmark

static void __run_benchmark(char *benchmark);
static char ** __get_benchmarks(int* size);
static void __save_results(const char *name, unsigned long long int clockcycles, int iterations);

extern int errno;

int main(int argc, char *argv[])
{
    int i, count;
    char **benchmarks = __get_benchmarks(&count);

    // loop through all of the benchmarks    
    for(i=0; i<count; i++)
    {
        __run_benchmark(benchmarks[i]);
    }
    return 0;
}

static char ** __get_benchmarks(int* size)
{
    int i;
    char** benchmarks = NULL;
    
    DIR *d;
    struct dirent *dir;
    int bench_cnt = 0;
    d = opendir(BENCHMARK_FOLDER);
    
    
    if (d > 0)
    {
        while ((dir = readdir(d)) != NULL)
        {
            // Reallocate the space to add a new benchmark name to the array
            char ** new_benchmarks = realloc(benchmarks, sizeof(char*) * (bench_cnt+1));
            if (new_benchmarks != NULL)
            {
                benchmarks = new_benchmarks;
                char *name = dir->d_name; // get the filename
                if (name[0] == '.')
                {
                    continue; // skip hidden files
                }
                size_t len = strlen(name);
                
                // Allocate space for the string itself
                benchmarks[bench_cnt] = (char*) malloc(sizeof(char)*len+1);
                strcpy(benchmarks[bench_cnt], name); // add the file to the benchmark set
                bench_cnt++;
            }
        }
        closedir(d);
    }

    *size = bench_cnt;
    return benchmarks;
}

/* private function to get clock cycle count */
static uint64_t __get_clk()
{
	unsigned int lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

/* private function to run a benchmark */
static void __run_benchmark(char *benchmark)
{
    int iterations = ITERATIONS;
    int i, prev_percent = 0;
    uint64_t start, end;
    unsigned long long int total_clk = 0L; // Total # of clock cycles
    static char *argv[] = {"", NULL};
    char path[strlen(BENCHMARK_FOLDER) + strlen(benchmark) + 1];
    strcpy(path, BENCHMARK_FOLDER);
    strcat(path, benchmark);
    printf("Running benchmark \"%s\" for %d iterations...\n", benchmark, iterations);
    
    for (i=0; i<iterations; i++)
    {
        // Print out the percent complete
        int percent = (i*100) / iterations;
        if (percent > (prev_percent + 5)) // Print every 5 percent at most
        {
            prev_percent = percent;
            printf("%d%% complete...\n", percent);
        }

        start = __get_clk();
        // Spawn child process
        pid_t pid = fork();
        if (pid == 0)   // Child process
        {
            if(execv(path, argv)<0);{
                    printf("Errno is %d\n", errno);
            }
            exit(-1);
        }
        else // Parent Process
        {
            waitpid(pid, 0, 0); // Wait for child
        }
        end = __get_clk();
        total_clk += (end - start);
    }
    printf("Benchmark Complete.\n");
    printf("Average number of clock cycles for %d iterations: %f million\n", 
        iterations, 
        (total_clk / iterations)/1000.0);
    __save_results(benchmark, total_clk, iterations);
}

static void __save_results(const char *name, unsigned long long int clockcycles, int iterations)
{
    FILE *fs = fopen(BENCHMARK_RESULT_FILE, "r"); // Check if file exists already
    if (fs == NULL)
    {
        // File must not exist... create one
        fs = fopen(BENCHMARK_RESULT_FILE, "w");
        if (fs == NULL)
        {
            // If it is still null, then something is wrong
            printf("ERROR opening results file!\n");
            return;
        }
        // Print the header since we just created the file
        fprintf(fs, "Benchmark Name,Clock Cycles,Iterations,Average (in millions of clock cycles)\n");
    }
    else
    {
        fclose(fs);
        // Open to append
        fs = fopen(BENCHMARK_RESULT_FILE, "a");
    }

    fprintf(fs, "%s,%llu,%d,%f\n", name, clockcycles, iterations, (clockcycles / iterations)/1000.0);
    fclose(fs);
    printf("Results saved to %s\n", BENCHMARK_RESULT_FILE);
}
