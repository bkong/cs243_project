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

int main(int argc, char *argv[])
{
    int i = 0;
    char **benchmarks = __get_benchmark_names();
    char bench_str[20];
    int iterations = 0;
    const char *SEP = ",";
    
    if (argc <= 1)
    {
        // User needs to select which benchmark to run
        printf("Choose which benchmark to run:\n");
        while(benchmarks[i])
        {
            printf("%d. %s\n", i+1, benchmarks[i]);
            i++;
        }
        printf("\nEnter number(s): ");
        scanf("%s", bench_str);
    }
    else if (argc == 2)
    {
        // User entered which benchmark as command line arg
        strcpy(bench_str, argv[1]);
    }
    else
    {
        // User entered which benchmark as command line arg
        strcpy(bench_str, argv[1]);
        sscanf(argv[2], "%d", &iterations);
    }
    
    if (!iterations)
    {
        printf("\nEnter number of iterations: ");
        scanf("%d", &iterations);
    }
    
    char* bench;
    char* benches = bench_str; // change to pointer to avoid warning
    // loop through all of the benchmarks, separated by commas
    while((bench = strsep(&benches, SEP)) != NULL) 
    {
        int benchnum = atoi(bench);
        __run_benchmark(benchmarks[benchnum-1], iterations);
    }
    return 0;
}

static char **__get_benchmark_names()
{
    char **names = malloc(sizeof(char*) * MAX_BENCHMARKS); // create array of char pointers
    DIR *d;
    int dircnt = 0;
    struct dirent *dir;
    d = opendir(BENCHMARK_FOLDER);
    
    if (d > 0)
    {
        while ((dir = readdir(d)) != NULL && dircnt < MAX_BENCHMARKS)
        {
            size_t ext_len = strlen(EXT);
            char *name = dir->d_name; // get the filename
            if (__is_benchmark_file(name))
            {
                size_t len = strlen(name);
                char bench_name[len -ext_len+1];
                strncpy(bench_name, name, len-ext_len); // remove the '.bnch' part
                bench_name[len-ext_len] = '\0'; // null terminate the string
                names[dircnt] = malloc(sizeof(char)*strlen(bench_name)+1);
                strcpy(names[dircnt], bench_name); // add the file to the array
                dircnt++;
            }
        }
        closedir(d);
    }
    
    return names;
}

static int __is_benchmark_file(const char *name)
{
    if (name != NULL)
    {
        size_t size = strlen(name); // size of filename
        size_t ext_size = strlen(EXT); // size of extension
        int i;
        
        if (size >= ext_size)
        {
            for(i=0; i<ext_size; i++)
            {
                // does current char == extension?
                if (name[size-ext_size+i] != EXT[i])
                {
                    return 0;
                }
            }
            return 1;
        }
    }
    
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
static void __run_benchmark(char *benchmark, int iterations)
{
    int i;
    uint64_t start, end;
    unsigned long long int total_clk = 0L; // Total # of clock cycles
    static char *argv[] = {"", NULL};
    char path[strlen(BENCHMARK_FOLDER) + strlen(benchmark) + strlen(EXT) + 1];
    strcpy(path, BENCHMARK_FOLDER);
    strcat(path, benchmark);
    strcat(path, EXT);
    printf("Running benchmark \"%s\" for %d iterations...\n", benchmark, iterations);
    
    for (i=0;i<iterations;i++)
    {
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
        
        total_clk += ((end - start) / 1000);
    }
    printf("Benchmark Complete.\n");
    printf("Average number of clock cycles for %d iterations: %f million\n", 
    iterations, 
    (total_clk / iterations)/1000.0);
}


