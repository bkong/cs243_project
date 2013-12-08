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

#define BENCHMARK_FOLDER "./compiled/"
#define BENCHMARK_RESULT_FILE "./benchmark_results.csv"
#define ITERATIONS 20 // Number of times to run each benchmark

static void __run_benchmark(char *benchmark);
static char ** __get_benchmarks(int* size);
static void __save_results(const char *name, unsigned long long int clockcycles, int iterations);
static char ** __get_optimizations(const char *bench_name);

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
        (total_clk / iterations)/1000000.0);
    __save_results(benchmark, total_clk, iterations);
}

static void __save_results(const char *name, unsigned long long int clockcycles, int iterations)
{
    char **benchmark_info = __get_optimizations(name);
    int column = 1; // column to write the result to
    
    FILE *fp = fopen(BENCHMARK_RESULT_FILE, "r"); // to read the file
    if (fp == NULL)
    {
        // File didnt exist before;
        fp = fopen(BENCHMARK_RESULT_FILE, "w"); // to write to file
        if (fp == NULL)
        {
            // If it is still null, then something is wrong
            printf("ERROR opening results file!\n");
            return;
        }
        // Print the header since we just created the file
        fprintf(fp, "Benchmark Name,Compiler Flag(s),Result\n");
    }
    else
    {
        fclose(fp);
        // Open to update
        fp = fopen(BENCHMARK_RESULT_FILE, "a");
    }

    fprintf(fp, "%s,%s,%f\n", benchmark_info[0], benchmark_info[1], (clockcycles / iterations)/1000.0);
    fclose(fp);
    free(benchmark_info[0]);
    free(benchmark_info[1]);
    free(benchmark_info);
    printf("Results saved to %s\n", BENCHMARK_RESULT_FILE);
}

// Returns an array of two elements containing
// [0]: Benchmark name
// [1]: Optimization flag
static char ** __get_optimizations(const char *bench_name)
{
    char **result = malloc(sizeof(char *) * 2);
    int i = 0;
    int full_length = strlen(bench_name);
    char c;
    
    do
    {
        c = bench_name[i++];
    }
    while( (c != '-') && (c != '\0') );
    
    result[0] = malloc(sizeof(char)*(i-1));
    strncpy(result[0], bench_name, i-1);
    result[0][i-1] = '\0'; // null terminate the string
    
    printf("bench: %s\n", result[0]);
    
    result[1] = malloc(sizeof(char)*(full_length - (i-3)));
    if ((full_length - i) > 0)
    {
        strcpy(result[1], &bench_name[i]);
    }
    else
    {
        strcpy(result[1], "");
    }
    
    printf("opti: %s\n", result[1]);
    
    return result;
}
