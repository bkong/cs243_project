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

#define MAX_BENCHMARKS 10
#define ITERATIONS 1000
#define BENCHMARK_FOLDER "./src/benchmarks/"

static void __run_benchmark(char *benchmark);
static int __is_benchmark_file(const char *name);
static char **__get_benchmark_names();

extern int errno;

int main(int argc, char *argv[])
{
    int i = 0;
    int bench_num = 0;
    char **benchmarks = __get_benchmark_names();
    
    if (argc <= 1)
    {
        // User needs to select which benchmark to run
        printf("Choose which benchmark to run:\n");
        while(benchmarks[i])
        {
            printf("%d. %s\n", i+1, benchmarks[i]);
            i++;
        }
        printf("\nEnter number: ");
        scanf("%d", &bench_num);
    }
    else
    {
        // User entered which benchmark as command line arg
        // TODO: allow benchmark name or number as arg
        if (sscanf(argv[1], "%d", &bench_num) != 1)
        {
            printf("Invalid benchmark number: %s\n", argv[1]);
            return -1;
        }
    }
    
    if (bench_num)
    {
        __run_benchmark(benchmarks[bench_num-1]);
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
        while ((dir = readdir(d)) != NULL)
        {
            char *name = dir->d_name; // get the filename
            if (__is_benchmark_file(name))
            {
                int len = strlen(name);
                char bench_name[len-1];
                strncpy(bench_name, name, len - 2); // remove the '.c' part
                bench_name[len-2] = '\0'; // null terminate the string
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
        size_t size = strlen(name);
        
        if ((size >= 2) &&
            (name[size-2] == '.') &&
            (name[size-1] == 'c'))
        {
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
static void __run_benchmark(char *benchmark)
{
    int i;
    uint64_t start, end;
    unsigned long long int total_clk = 0L; // Total # of clock cycles
    static char *argv[] = {"", NULL};
    char path[strlen(BENCHMARK_FOLDER) + strlen(benchmark) + 1];
    strcpy(path, BENCHMARK_FOLDER);
    strcat(path, benchmark);
    printf("Starting benchmark \"%s\"...\n", benchmark);
    
    
    for (i=0;i<ITERATIONS;i++)
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
        
        total_clk += (end - start);
    }
    printf("Benchmark Complete.\n");
    printf("Average number of clock cycles for %d iterations: %f million\n", ITERATIONS, (total_clk / ITERATIONS)/1000000.0);
}


